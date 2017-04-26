#pragma once

#include "mu-core/Ranges.h"
#include "mu-core/Algorithms.h"

namespace mu {
	template<typename T, size_t MAX>
	class FixedArrayBase {
	protected:
		static constexpr size_t DataSize = sizeof(T) * MAX;
		u8 m_data[DataSize];
		size_t m_num = 0;
	};

	template<typename T, size_t MAX, bool TRIVIAL_DESTRUCTOR>
	class FixedArrayDestructorMixin : public FixedArrayBase<T, MAX> {
	};

	template<typename T, size_t MAX>
	class FixedArrayDestructorMixin<T, MAX, false> : public FixedArrayBase<T, MAX> {
	protected:
		~FixedArrayDestructorMixin() {
			T* start = (T*)m_data;
			T* end = start + m_num;
			Destroy(Range(start, end));
		}
	};

	template<typename T, size_t MAX>
	class FixedArray : protected FixedArrayDestructorMixin<T, MAX, std::is_trivially_destructible_v<T>> {

	public:
		FixedArray() {}


		size_t Num() const { return m_num; }
		T* Data() { return (T*)m_data; }
		const T* Data() const { return (T*)m_data; }
		T& operator[](size_t index) { return ((T*)m_data)[index]; }
		const T& operator[](size_t index) const { return ((T*)m_data)[index]; }

		void Add(const T& element) {
			new(AddInternal()) T(element);
		}
		void Empty() {
			for (size_t i = 0; i < m_num; ++i) {
				(*this)[i].~T();
			}
			m_num = 0;
		}

		auto begin() { return mu::MakeRangeIterator(mu::Range((T*)m_data, m_num)); }
		auto end() { return mu::MakeRangeSentinel(); }

		auto begin() const { return mu::MakeRangeIterator(mu::Range((T*)m_data, m_num)); }
		auto end() const { return mu::MakeRangeSentinel(); }

	private:
		T* AddInternal() {
			T* t = ((T*)m_data) + m_num;
			++m_num;
			return t;
		}
	};
}
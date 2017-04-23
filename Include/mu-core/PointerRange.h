#pragma once

#include "mu-core/RangeIteration.h"

namespace mu {
// A linear forward range over raw memory of a certain type
	template<typename T>
	class PointerRange : public details::WithBeginEnd<PointerRange<T>> {
		T* m_start, *m_end;

	public:
		static constexpr bool HasSize = true;
		static constexpr bool IsContiguous = true;

		template<class U>
		friend class PointerRange;

		PointerRange()
			: m_start(nullptr)
			, m_end(nullptr) {}

		PointerRange(T* start, T* end)
			: m_start(start)
			, m_end(end) {}

		template<typename U>
		PointerRange(PointerRange<U> other) {
			m_start = other.m_start;
			m_end = other.m_end;
		}

		void Advance() { ++m_start; }
		void AdvanceBy(size_t num) { m_start += num; }

		bool IsEmpty() const { return m_start >= m_end; }
		T& Front() { return *m_start; }
		const T& Front() const { return *m_start; }
		size_t Size() const { return m_end - m_start; }

		template<typename U>
		bool operator==(const PointerRange<U>& other) const {
			return m_start == other.m_start
				&& m_end == other.m_end;
		}

		template<typename U>
		bool operator!=(const PointerRange<U>& other) const {
			return m_start != other.m_start
				|| m_end != other.m_end;
		}

		PointerRange MakeEmpty() const { return PointerRange{ nullptr, nullptr }; }
	};
}
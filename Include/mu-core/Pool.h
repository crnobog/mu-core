#pragma once

#include "Array.h"
#include "BitArray.h"
#include "PrimitiveTypes.h"
#include "Debug.h"

#include <intrin.h>

namespace mu {

	// Externally synchronized fixed-size pool
	template<typename ElementType, typename IndexType=size_t>
	class Pool {
		ElementType* m_elements = nullptr;
		BitArray m_flags;
		size_t m_max = 0;
		size_t m_free = 0;

	public:
		Pool() {}

		Pool(size_t max_elements) : m_max(max_elements) {
			m_elements = (ElementType*)malloc(max_elements * sizeof(ElementType));
			m_flags.Init(max_elements, false);
			m_free = max_elements;
		}

		~Pool() {
			Empty();
		}

		Pool(const Pool& other) = delete;
		Pool(Pool&& other) = delete;
		Pool& operator=(const Pool& other) = delete;
		Pool& operator=(Pool&& other) = delete;

		size_t GetFreeCount() const { return m_free; }

		IndexType AddDefaulted() {
			size_t index = AllocateIndex();
			new(&m_elements[index]) ElementType;
			return IndexType{ index };
		}

		template<typename... TS>
		IndexType Emplace(TS&&... ts) {
			size_t index = AllocateIndex();
			new(&m_elements[index]) ElementType(std::forward<TS>(ts)...);
			return IndexType{ index };
		}

		void Return(IndexType i) {
			size_t index = (size_t)i;
			CHECK(m_flags.GetBit(index));
			CHECK(index < m_max);
			++m_free;
			m_flags.ClearBit(index);
			m_elements[index].~ElementType();
		}

		void Empty() {
			for (size_t index : m_flags.GetSetBits()) {
				m_elements[index].~ElementType();
			}

			if (m_elements) { free(m_elements); }
			m_elements = nullptr;
			m_flags.Empty();
			m_max = 0;
			m_free = 0;
		}

		ElementType& operator[](IndexType index) {
			return m_elements[(size_t)index];
		}
		const ElementType& operator[](IndexType index) const {
			return m_elements[(size_t)index];
		}

	protected:
		size_t AllocateIndex() {
			CHECK(m_free > 0);
			--m_free;
			auto r = m_flags.GetClearBits();
			CHECK(!r.IsEmpty());
			
			size_t index = r.Front();
			m_flags.SetBit(index);
			return index;
		}
	};
}
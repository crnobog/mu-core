#pragma once

namespace mu {
// A linear infinite range over an integral type
	template<typename T>
	class IotaRange {
		T m_it = 0;
	public:
		enum { HasSize = 0 };

		IotaRange(T start = 0) : m_it(start) {}

		void Advance() { ++m_it; }
		bool IsEmpty() const { return false; }
		T Front() { return m_it; }
	};

	template<typename T = size_t>
	inline auto Iota(T start = 0) { return IotaRange<T>(start); }

	template<typename T>
	inline auto Range(IotaRange<T> r) { return r; }
}

#ifdef DOCTEST_LIBRARY_INCLUDED
#include "Tests/IotaRange_Tests.inl"
#endif
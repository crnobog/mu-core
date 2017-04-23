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
}
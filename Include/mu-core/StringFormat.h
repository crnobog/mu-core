#pragma once

#include "mu-core/String.h"
#include <cstdint>

enum class StringFormatArgType {
	None,
	C_Str,
	Unsigned
};

struct StringFormatArg {
	StringFormatArgType m_type;
	union {
		const char* m_c_str;
		uint64_t m_uint;
	};

	StringFormatArg(const char* c_str)
		: m_type(StringFormatArgType::C_Str)
		, m_c_str(c_str) {}

	StringFormatArg(const String_T<char>& str) {
		if (str.IsEmpty()) {
			m_type = StringFormatArgType::None;
		}
		else {
			m_type = StringFormatArgType::C_Str;
			m_c_str = str.GetRaw();
		}
	}

	StringFormatArg(int32_t i32)
		: m_type(StringFormatArgType::Unsigned)
		, m_uint(i32) {}

	StringFormatArg(uint32_t u32)
		: m_type(StringFormatArgType::Unsigned)
		, m_uint(u32) {}

	StringFormatArg(size_t s)
		: m_type(StringFormatArgType::Unsigned)
		, m_uint(s) {}
};
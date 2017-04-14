#pragma once

#include "mu-core/String.h"
#include <cstdint>

enum class StringFormatArgType {
	None,
	C_Str,
	Unsigned
};

namespace mu {
	template<typename T> class String_T;

	struct StringFormatArg {
		StringFormatArgType m_type;
		union {
			const char* m_c_str;
			uint64_t m_uint;
		};

		StringFormatArg(const char* c_str);
		StringFormatArg(const String_T<char>& str);
		StringFormatArg(int32_t i32);
		StringFormatArg(uint32_t u32);
		StringFormatArg(size_t s);
	};
}

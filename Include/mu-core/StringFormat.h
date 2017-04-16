#pragma once

#include "mu-core/PrimitiveTypes.h"
#include <tuple>

enum class StringFormatArgType {
	None,
	C_Str,
	Unsigned
};

namespace mu {
	template<typename T> class String_T;

	struct StringFormatArg {
		StringFormatArgType m_type = StringFormatArgType::None;
		union {
			std::tuple<const char*, size_t> m_c_str;
			u64 m_uint;
		};
		StringFormatArg() {}
		StringFormatArg(const StringFormatArg& other);
		StringFormatArg(StringFormatArg&& other);
		StringFormatArg(const char* c_str);
		StringFormatArg(const String_T<char>& str);
		StringFormatArg(i32 i);
		StringFormatArg(u32 u);
		StringFormatArg(size_t s);
	};
}

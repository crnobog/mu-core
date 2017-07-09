#pragma once

#include "mu-core/PrimitiveTypes.h"
#include "mu-core/Metaprogramming.h"
#include <tuple>

enum class StringFormatArgType {
	None,
	C_Str,
	Unsigned,
	Double,
};

namespace mu {
	template<typename T> class String_T;

	struct StringFormatArg {
		StringFormatArgType m_type = StringFormatArgType::None;
		union {
			std::tuple<const char*, size_t> m_c_str;
			u64 m_uint;
			double m_double;
		};
		StringFormatArg() {}
		StringFormatArg(const StringFormatArg& other);
		StringFormatArg(StringFormatArg&& other);
		StringFormatArg(const char* c_str);
		StringFormatArg(const String_T<char>& str);
		StringFormatArg(i32 i);
		StringFormatArg(u32 u);
		StringFormatArg(float f);
		StringFormatArg(double d);
		StringFormatArg(size_t s);

		template<typename RANGE, EnableIf<RANGE::IsContiguous && RANGE::HasSize>...>
		StringFormatArg(RANGE&& r)
			: m_c_str(&r.Front(), r.Size())
			, m_type(StringFormatArgType::C_Str)
		{}
	};
}

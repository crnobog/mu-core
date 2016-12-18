#pragma once

#include "mu-core/Array.h"

// TODO: Handle multi-character encodings. We'll just use utf-32 in memory for now
// TODO: Encoding is a template parameter so conversions can be done automatically
template<typename CharType>
class String_T
{
	Array<CharType> m_data;

public:
	String_T() = default;
	~String_T() = default;
	String_T(const String_T&) = default;
	String_T(String_T&&) = default;
	String_T& operator=(const String_T&) = default;
	String_T& operator=(String_T&&) = default;

	const Array<CharType>& GetData() const { return m_data; }

	bool operator==(const String_T& other) const
	{
		for( auto&& pair : Zip(m_data, other.m_data) )
		{
			if( std::get<0>(pair) != std::get<1>(pair))
			{
				return false;
			}
		}
		return true;
	}
};

// UTF-8 string
using String = String_T<char>;
// UTF-32 string
using String32 = String_T<char32_t>;
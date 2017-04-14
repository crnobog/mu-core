#pragma once

#include "mu-core/Array.h"
#include "mu-core/Ranges.h"
#include "mu-core/StringFormat.h"

#include <array>

namespace mu {
// #TODO: Encoding is a template parameter so conversions can be done automatically
	template<typename CharType>
	class String_T {
		Array<CharType> m_data;

	public:
		String_T() = default;
		~String_T() = default;
		String_T(const String_T&) = default;
		String_T(String_T&&) = default;
		String_T& operator=(const String_T&) = default;
		String_T& operator=(String_T&&) = default;

		String_T(const CharType* str) {
			for (; *str != '\0'; ++str) {
				m_data.Add(*str);
			}
			m_data.Add('\0');
		}

		String_T(const Array<CharType>& str) {
			m_data.Append(str);
			if (m_data[m_data.Num() - 1] != '\0') {
				m_data.Add('\0');
			}
		}

		template<typename OtherChar, typename SizeType>
		String_T(std::tuple<OtherChar*, SizeType> t) {
			const auto* str = std::get<0>(t);
			auto len = std::get<1>(t);
			m_data.Reserve(len + 1);
			for (CharType c : mu::Range(str, len)) {
				m_data.Add(c);
			}
			m_data.Add('\0');
		}

		bool IsEmpty() const { return m_data.Num() <= 1; }
		const CharType* GetRaw() const { return m_data.Data(); }
		size_t GetLength() const {
			size_t n = m_data.Num();
			return n == 0 ? 0 : n - 1;
		}

		bool operator==(const String_T& other) const {
			for (auto&& pair : mu::Zip(m_data, other.m_data)) {
				if (std::get<0>(pair) != std::get<1>(pair)) {
					return false;
				}
			}
			return true;
		}

		void Add(CharType c) {
			if (m_data.Num() == 0) {
				m_data.Add(c);
			}
			else {
				m_data[m_data.Num() - 1] = c;
			}
			m_data.Add('\0');
		}
		void Append(const CharType* c) {
			if (m_data.Num() > 0) {
				m_data.RemoveAt(m_data.Num() - 1);
			}
			for (; *c != '\0'; ++c) {
				m_data.Add(*c);
			}
			m_data.Add('\0');
		}
		void Append(const String_T& s) {
			if (m_data.Num() > 0) {
				m_data.RemoveAt(m_data.Num() - 1);
			}
			m_data.Append(s.GetData());
		}

		template<typename RANGE, typename std::enable_if<!RANGE::HasSize, int>::type = 0>
		void Append(RANGE&& r) {
			for (; !r.IsEmpty(); r.Advance()) {
				Add(r.Front());
			}
		}

		template<typename RANGE, typename std::enable_if<RANGE::HasSize, int>::type = 0>
		void Append(RANGE&& r) {
			m_data.Reserve(r.Size());
			for (; !r.IsEmpty(); r.Advance()) {
				Add(r.Front());
			}
		}


		int Compare(const char* c_str) const {
			return strncmp(m_data.Data(), c_str, m_data.Num());
		}

		bool operator!=(const String_T& other) {
			if (IsEmpty() != other.IsEmpty()) {
				return false;
			}
			return Compare(other.GetRaw()) != 0;
		}

		bool operator==(const String_T& other) {
			if (IsEmpty() != other.IsEmpty()) {
				return false;
			}
			return Compare(other.GetRaw()) == 0;
		}
	};

	// UTF-8 string
	using String = String_T<char>;
	// UTF-32 string
	using String32 = String_T<char32_t>;

	template<typename CharType>
	PointerRange<const CharType> Range(const String_T<CharType>& s) {
		return Range(s.GetRaw(), s.GetLength()-1);
	}
	template<typename CharType>
	PointerRange<const CharType> Range(String_T<CharType>& s) {
		return Range(s.GetRaw(), s.GetLength()-1);
	}

	template<typename... ARGS>
	String Format(ARGS&&... args) {
		auto arr = std::array<StringFormatArg, sizeof...(args)>{ { StringFormatArg(std::forward<ARGS>(args))... }};
		String s;
		char buffer[256];
		for (StringFormatArg& arg : arr) {
			switch (arg.m_type) {
			case StringFormatArgType::C_Str:
				s.Append(arg.m_c_str);
				break;
			case StringFormatArgType::Unsigned:
				auto count = snprintf(buffer, 256, "%I64u", arg.m_uint);
				if (count > 0) {
					s.Append(buffer);
				}
				break;
			}
		}
		return s;
	}
}
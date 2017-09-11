#include "mu-core/StringFormat.h"
#include <stdexcept>

namespace mu {
	void ValidateFormatString(const char* fmt) {
		ValidateFormatString(fmt, {});
	}

	void ValidateFormatString(const char* fmt, PointerRange<StringFormatArgType> arg_types) {
		// No args overload, the format string should contain no {} blocks

		size_t expected_args = 0;
		for (const char* cursor = fmt; *cursor != '\0'; ++cursor) {
			if (*cursor != '{') { continue; }
			++cursor;
			if (*cursor == '{') {
				// Escaped brace
				continue;
			}
			else if (*cursor == '}') {
				++expected_args;
			}
		}

		if (expected_args != arg_types.Size()) {
			throw std::runtime_error("Format string mismatch");
		}
	}
}

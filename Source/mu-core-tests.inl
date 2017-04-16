#pragma once
#include "CppUnitTest.h"

#include "mu-core/String.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			std::wstring ToString(const mu::String& s) {
				auto w = mu::UTF8StringToWide(Range(s));
				return std::wstring(w.GetRaw());
			}
		}
	}
}
#include "CppUnitTest.h"
#include "mu-core/String.h"
#include "mu-core/Ranges.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			std::wstring ToString(const mu::String& s) {
				return L"";
			}
		}
	}
}
namespace mu_core_tests_string {
	using namespace mu;

	TEST_CLASS(StringConstructTests) {
public:
	TEST_METHOD(FromCString) {
		String s{ "test" };
		Assert::AreEqual<size_t>(4, s.GetLength());
	}
	TEST_METHOD(FromTuple) {
		const char t[] = "test";
		std::tuple<const char*, size_t> tup{ t, sizeof(t) - 1 };
		String s{ tup };
		Assert::AreEqual<size_t>(4, s.GetLength());
	}
	TEST_METHOD(FromArray) {
		Array<char> a;
		const char t[] = "test";
		for (const char* c = t; ; ++c) {
			a.Add(*c);
			if (*c == '\0')
				break;
		}
		String s{ a };
		Assert::AreEqual<size_t>(4, s.GetLength());
	}
	};

	TEST_CLASS(StringFormatTests) {
public:
	TEST_METHOD(Strings) {
		String a{ "testA" };
		String b{ "testB" };

		String c = Format(a, b);
		String expected = "testAtestB";
		Assert::AreEqual<String>(expected, c);
	}
	TEST_METHOD(Int) {
		String s = Format(100);
		String expected = "100";
		Assert::AreEqual<String>(expected, s);
	}
	};
}
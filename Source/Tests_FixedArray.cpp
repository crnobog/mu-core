#include "mu-core-tests.inl"
#include "mu-core/FixedArray.h"

namespace mu_core_tests_fixedarray {
	using namespace mu;

	struct EmptyElement {};
	struct Element {
		i32 Data;
	};

	TEST_CLASS(FixedArrayConstructTests) {
public:
	TEST_METHOD(Empty) {
		FixedArray<EmptyElement, 10> arr;
	}
	};

	TEST_CLASS(FixedArrayIterateTests) {
	public:
		TEST_METHOD(RangeFor) {
			FixedArray<Element, 10> arr;
			for (i32 i = 0; i < 5; ++i) {
				arr.Add({ i });
			}

			i32 i = 0;
			for (Element& e : arr) {
				Assert::AreEqual(i, e.Data);
				++i;
			}
		}
	};
}
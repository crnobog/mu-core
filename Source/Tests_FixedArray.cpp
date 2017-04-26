#include "mu-core-tests.inl"
#include "mu-core/FixedArray.h"

namespace mu_core_tests_fixedarray {
	using namespace mu;

	i32 ElementConstructorCount = 0;
	i32 ElementCopyConstructorCount = 0;
	i32 ElementDestructorCount = 0;

	void ResetElementCounts() {
		ElementConstructorCount = 0;
		ElementCopyConstructorCount = 0;
		ElementDestructorCount = 0;
	}

	struct EmptyElement {};
	struct Element {
		i32 Data;

		Element() { ElementConstructorCount++; }
		Element(i32 i) { ElementConstructorCount++; Data = i; }
		Element(const Element& e) { ElementCopyConstructorCount++; Data = e.Data; }
		~Element() { ElementDestructorCount++; }
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

	TEST_CLASS(FixedArrayItemDestructorTests) {
	public:
		TEST_METHOD_INITIALIZE(Reset) {
			ResetElementCounts();
		}

		TEST_METHOD(NoDestructor) {
			FixedArray<EmptyElement, 10> arr;

			static_assert(std::is_trivially_destructible_v<EmptyElement> == true, "EmptyElement is not trivially destructible");
			static_assert(std::is_trivially_destructible_v<decltype(arr)> == true, "FixedArray<EmptyElement> is not trivially destructible");
		}
		TEST_METHOD(Empty) {
			FixedArray<Element, 10> arr;

			static_assert(std::is_trivially_destructible_v<decltype(arr)> == false, "FixedArray<Element> is trivially destructible");

			Assert::AreEqual(0, ElementConstructorCount);
			Assert::AreEqual(0, ElementDestructorCount);
		}

		TEST_METHOD(ArrayLeaveScope) {
			{
				FixedArray<Element, 10> arr;
			}
			Assert::AreEqual(0, ElementConstructorCount);
			Assert::AreEqual(0, ElementDestructorCount);
		}

		TEST_METHOD(ArrayLeaveScopeWithContents) {
			{
				FixedArray<Element, 10> arr;
				arr.Add({ 5 });
				ResetElementCounts();
			}
			Assert::AreEqual(0, ElementConstructorCount, L"constructor count mismatch");
			Assert::AreEqual(0, ElementCopyConstructorCount, L"copy constructor count mismatch");
			Assert::AreEqual(1, ElementDestructorCount, L"destructor count mismatch");
		}
	};
}
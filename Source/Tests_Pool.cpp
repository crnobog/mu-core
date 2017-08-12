#include "CppUnitTest.h"
#include "mu-core/Pool.h"
#include "mu-core/PrimitiveTypes.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mu_core_tests_pool
{
	using namespace mu;

	static int ConstructCount = 0;
	static int DestructCount = 0;
	static int CopyCount = 0;
	static int MoveCount = 0;

	static void ResetCounts() {
		ConstructCount = 0;
		DestructCount = 0;
		CopyCount = 0;
		MoveCount = 0;
	}

	struct Element {
		i32 data;

		Element() : data(0) {
			++ConstructCount;
		}
		Element(i32 d) : data(d) {
			++ConstructCount;
		}
		Element(const Element& other) : data(other.data) {
			++CopyCount;
		}
		Element(Element&& other) : data(other.data) {
			++MoveCount;
		}
		Element& operator=(const Element& other) {
			++CopyCount;
			return *this;
		}
		Element& operator=(Element&& other) {
			++MoveCount;
			return *this;
		}
		~Element() {
			++DestructCount;
		}
	};

	TEST_CLASS(PoolTests)
	{
		TEST_METHOD_INITIALIZE(MethodInit) {
			ResetCounts();
		}

		TEST_METHOD(AddToLimit)
		{
			Pool<Element> p{ 4 };
			size_t a = p.AddDefaulted();
			size_t b = p.AddDefaulted();
			size_t c = p.AddDefaulted();
			size_t d = p.AddDefaulted();
			Assert::AreEqual(0, (i32)p.GetFreeCount());

			Assert::AreNotEqual(a, b);
			Assert::AreNotEqual(a, c);
			Assert::AreNotEqual(a, d);

			Assert::AreNotEqual(b, c);
			Assert::AreNotEqual(b, d);

			Assert::AreNotEqual(c, d);
		}

		TEST_METHOD(AddToLimitReleaseAddMore)
		{
			Pool<Element> p{ 4 };
			{
				size_t a = p.AddDefaulted();
				size_t b = p.AddDefaulted();
				size_t c = p.AddDefaulted();
				size_t d = p.AddDefaulted();
				Assert::AreEqual(0, (i32)p.GetFreeCount());

				p.Return(a);
				p.Return(b);
				p.Return(c);
				p.Return(d);

				Assert::AreEqual(4, (i32)p.GetFreeCount());
			}

			size_t a = p.AddDefaulted();
			size_t b = p.AddDefaulted();
			size_t c = p.AddDefaulted();
			size_t d = p.AddDefaulted();
			Assert::AreEqual(0, (i32)p.GetFreeCount());

			Assert::AreNotEqual(a, b);
			Assert::AreNotEqual(a, c);
			Assert::AreNotEqual(a, d);

			Assert::AreNotEqual(b, c);
			Assert::AreNotEqual(b, d);

			Assert::AreNotEqual(c, d);
		}

		TEST_METHOD(OnlyAddedObjectsAreDestroyed) {
			{
				Pool<Element> p{ 128 };
			}

			Assert::AreEqual(0, ConstructCount);
			Assert::AreEqual(0, DestructCount);

			{
				Pool<Element> p{ 128 };

				size_t a = p.AddDefaulted();
				size_t b = p.AddDefaulted();
				size_t c = p.AddDefaulted();
				ResetCounts();
			}
			
			Assert::AreEqual(3, DestructCount);
		}

		TEST_METHOD(RemovedObjectsAreDestroyedImmediately) {
			{
				Pool<Element> p{ 128 };

				size_t a = p.AddDefaulted();
				size_t b = p.AddDefaulted();
				size_t c = p.AddDefaulted();
				ResetCounts();
				p.Return(a);
				p.Return(b);

				Assert::AreEqual(2, DestructCount);
			}
		}

		TEST_METHOD(RemovedObjectsAreNotDestroyedOnPoolDestruction) {
			{
				Pool<Element> p{ 128 };

				size_t a = p.AddDefaulted();
				size_t b = p.AddDefaulted();
				size_t c = p.AddDefaulted();
				p.Return(a);
				p.Return(b);
				
				ResetCounts();
			}

			Assert::AreEqual(1, DestructCount);
		}
	};
}
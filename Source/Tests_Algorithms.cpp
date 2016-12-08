#include "CppUnitTest.h"
#include "mu-core/Ranges.h"
#include "mu-core/Algorithms.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace mu_core_tests_algorithms
{
	using namespace mu;

	TEST_CLASS(MoveTests)
	{
		static int ConstructCount;
		static int DestructCount;
		static int CopyCount;
		static int MoveCount;
		static int CopyAssignCount;
		static int MoveAssignCount;

		static void ResetCounts()
		{
			ConstructCount = 0;
			DestructCount = 0;
			CopyCount = 0;
			MoveCount = 0;
			CopyAssignCount = 0;
			MoveAssignCount = 0;
		}

		struct Element
		{
			int32_t data;

			Element() : data(0)
			{
				++ConstructCount;
			}
			Element(int32_t d) : data(d)
			{
				++ConstructCount;
			}
			Element(const Element& other) : data(other.data)
			{
				++CopyCount;
			}
			Element(Element&& other) : data(other.data)
			{
				++MoveCount;
			}
			Element& operator=(const Element& other)
			{
				++CopyAssignCount;
				return *this;
			}
			Element& operator=(Element&& other)
			{
				++MoveAssignCount;
				return *this;
			}
			~Element()
			{
				++DestructCount;
			}
		};
	public:
		TEST_METHOD_INITIALIZE(MethodInit)
		{
			ResetCounts();
		}

		TEST_METHOD(MovePrimitive)
		{
			int from[] = { 0,1,2,3,4,5,6,7,8,9 };
			int to[20] = {};

			auto source = Range(from);
			auto dest = Range(to);
			size_t source_size_start = source.Size(), dest_size_start=dest.Size();
			Assert::AreEqual(size_t(10), source.Size(), nullptr, LINE_INFO());
			Assert::AreEqual(size_t(20), dest.Size(), nullptr, LINE_INFO());

			auto dest2 = Move(dest, source);
			Assert::AreEqual(dest_size_start, dest.Size());
			Assert::AreEqual(source_size_start, source.Size());
			Assert::AreEqual(dest.Size() - source.Size(), dest2.Size(), nullptr, LINE_INFO());
			Assert::AreEqual(size_t(10), source.Size(), nullptr, LINE_INFO());

			auto dest3 = Move(dest2, source);
			Assert::AreEqual(dest.Size() - source.Size() * 2, dest3.Size(), nullptr, LINE_INFO());
			Assert::IsTrue(dest3.IsEmpty(), nullptr, LINE_INFO());

			int index = 0;
			for (auto r = Range(to); !r.IsEmpty(); r.Advance(), ++index)
			{
				Assert::AreEqual(from[index % 10], r.Front(), nullptr, LINE_INFO());
			}
		}

		TEST_METHOD(MoveConstructPrimitive)
		{
			int from[] = { 0,1,2,3,4,5,6,7,8,9 };
			int to[20] = {};

			auto source = Range(from);
			auto dest = Range(to);
			Assert::AreEqual(size_t(10), source.Size(), nullptr, LINE_INFO());
			Assert::AreEqual(size_t(20), dest.Size(), nullptr, LINE_INFO());

			auto dest2 = MoveConstruct(dest, source);
			Assert::AreEqual(dest.Size() - source.Size(), dest2.Size(), nullptr, LINE_INFO());
			Assert::AreEqual(size_t(10), source.Size(), nullptr, LINE_INFO());

			auto dest3 = MoveConstruct(dest2, source);
			Assert::AreEqual(dest.Size() - source.Size() * 2, dest3.Size(), nullptr, LINE_INFO());
			Assert::IsTrue(dest3.IsEmpty(), nullptr, LINE_INFO());

			int index = 0;
			for (auto r = Range(to); !r.IsEmpty(); r.Advance(), ++index)
			{
				Assert::AreEqual(from[index % 10], r.Front(), nullptr, LINE_INFO());
			}
		}

		TEST_METHOD(MoveObject)
		{
			Element es[10] = {};
			Element dest[10] = {};
			Assert::AreEqual(20, ConstructCount);

			Move(dest, es);
			Assert::AreEqual(20, ConstructCount);
			Assert::AreEqual(10, MoveAssignCount);
			Assert::AreEqual(0, MoveCount);
			Assert::AreEqual(0, DestructCount);
		}

		TEST_METHOD(MoveConstructObject)
		{
			Element es[10] = {};
			Element dest[10] = {};
			Assert::AreEqual(20, ConstructCount);

			MoveConstruct(dest, es);
			Assert::AreEqual(20, ConstructCount);
			Assert::AreEqual(0, MoveAssignCount);
			Assert::AreEqual(10, MoveCount);
			Assert::AreEqual(0, DestructCount);
		}
	};

	int MoveTests::ConstructCount;
	int MoveTests::DestructCount;
	int MoveTests::CopyCount;
	int MoveTests::MoveCount;
	int MoveTests::CopyAssignCount;
	int MoveTests::MoveAssignCount;

	TEST_CLASS(MapTests)
	{
	public:
		TEST_METHOD_INITIALIZE(MethodInit)
		{
		}

		TEST_METHOD(MapLambda)
		{
			int arr[] = { 1,2,3,4 };
			int expected[] = { 2,4,6,8 };
			Map(arr, [](int& a) { a *= 2; });

			for (auto r = Range(arr), x = Range(expected); !r.IsEmpty(); r.Advance(), x.Advance())
			{
				Assert::AreEqual(x.Front(), r.Front());
			}
		}

		TEST_METHOD(MapLambdaConst)
		{
			int sum = 0;
			int arr[] = { 5, 10, 20 };
			Map(arr, [&](const int&a) {sum += a; });

			Assert::AreEqual(35, sum);
		}

		TEST_METHOD(MapLambdaOriginalRangeNotAdvanced)
		{
			int arr[] = { 1,2,3,4 };
			auto initial = Range(arr);

			Map(initial, [](int& a) { a++; });

			Assert::IsTrue(Range(arr) == initial);
		}
	};

	TEST_CLASS(FindTests)
	{
	public:
		TEST_METHOD(ReturnsNonEmptyIterator)
		{
			int arr[] = { 10, 20, 100, 50, 40, 6, 100, 120, 50 };
			auto r= Find(arr, [](int a) { return a < 10; });

			Assert::IsFalse(r.IsEmpty());
		}

		TEST_METHOD(ReturnsEmptyIterator)
		{
			int arr[] = { 10, 20, 100, 50, 40, 100, 120, 50 };
			auto r = Find(arr, [](int a) { return a < 10; });

			Assert::IsTrue(r.IsEmpty());
		}

		TEST_METHOD(IteratorMatchesManualAdvance)
		{
			int arr[] = { 10, 20, 100, 50, 40, 100, 120, 50 };
			auto found = Find(arr, [](int a) { return a == 100; });

			auto r = Range(arr);
			r.Advance(); r.Advance();
			Assert::IsTrue(r == found);
		}
		
		TEST_METHOD(SucessiveFinds)
		{
			int arr[] = { 10, 20, 100, 50, 40, 100, 120, 50 };
			auto found = Find(arr, [](int a) { return a == 100; });

			auto first = found;
			found.Advance();
			auto second = Find(found, [](int a) { return a == 100;  });

			Assert::IsTrue(first != second);

			auto also_second = FindNext(first, [](int a) { return a == 100;  });
			Assert::IsTrue(second == also_second);
		}
	};
}
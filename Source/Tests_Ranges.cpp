#include "CppUnitTest.h"
#include "mu-core/Ranges.h"
#include "mu-core/Algorithms.h"

#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft{ namespace VisualStudio {namespace CppUnitTestFramework {
	template<> inline std::wstring ToString<char32_t>(const char32_t& t) { RETURN_WIDE_STRING((uint32_t)t); }
}}}

namespace mu_core_tests_ranges
{
	using namespace mu;

	TEST_CLASS(PointerRangeTests)
	{

	public:
		TEST_METHOD(Size)
		{
			const size_t size = 100;
			int ptr[size] = {};
			{
				auto r = Range(ptr, size);

				Assert::IsTrue(r.HasSize, nullptr, LINE_INFO());
				Assert::AreEqual(size, r.Size(), nullptr, LINE_INFO());
			}
			{
				auto r = Range(ptr, ptr + size);

				Assert::IsTrue(r.HasSize, nullptr, LINE_INFO());
				Assert::AreEqual(size, r.Size(), nullptr, LINE_INFO());
			}
			{
				auto r = Range(ptr);

				Assert::IsTrue(r.HasSize, nullptr, LINE_INFO());
				Assert::AreEqual(size, r.Size(), nullptr, LINE_INFO());
			}
		}

		TEST_METHOD(Reading)
		{
			int arr[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
			auto r = Range(arr);
			
			Assert::IsTrue(std::is_same<int&, decltype(r.Front())>::value);
			int i = 0;
			for (; !r.IsEmpty(); r.Advance(), ++i)
			{
				Assert::AreEqual(i * 2, r.Front());
			}
		}

		TEST_METHOD(Writing)
		{
			int arr[] = { 0, 1, 2, 3 };
			auto r = Range(arr);

			for (; !r.IsEmpty(); r.Advance())
			{
				int old = r.Front();
				r.Front() = r.Front() * 2;
				Assert::AreEqual(old * 2, r.Front());
			}
		}

		TEST_METHOD(ConstRange)
		{
			const int arr[] = { 0, 1, 2, 3, 4 };
			auto r = Range(arr);

			Assert::IsTrue(std::is_same<const int&, decltype(r.Front())>::value);
		}

		TEST_METHOD(ZipRanges)
		{
			int as[] = { 0,1,2,3,4,5,6,7,8,9 };
			float bs[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

			int index = 0;
			auto r = Zip(Range(as), Range(bs));
			Assert::IsTrue(r.HasSize, nullptr, LINE_INFO());
			Assert::IsFalse(r.IsEmpty(), nullptr, LINE_INFO());
			Assert::AreEqual(size_t(10), r.Size(), nullptr, LINE_INFO());

			for (; !r.IsEmpty(); r.Advance(), ++index)
			{
				std::tuple<int&, float&> front = r.Front();
				Assert::AreEqual(as[index], std::get<0>(front), nullptr, LINE_INFO());
				Assert::AreEqual(bs[index], std::get<1>(front), nullptr, LINE_INFO());
			}
		}

		TEST_METHOD(IotaRange)
		{
			size_t i = 0;
			auto r = Iota();
			Assert::IsFalse(r.HasSize);
			for (; !r.IsEmpty() && i < 10; ++i, r.Advance())
			{
				Assert::AreEqual(i, r.Front());
			}
		}

		TEST_METHOD(ZipIotas)
		{
			size_t i = 0;
			auto r = Zip(Iota(), Iota(1));
			Assert::IsFalse(r.HasSize);
			for (; !r.IsEmpty() && i < 10; ++i, r.Advance())
			{
				std::tuple<size_t, size_t> f = r.Front();
				Assert::AreEqual(1+ std::get<0>(f), std::get<1>(f));
			}
		}

		TEST_METHOD(ZipIotaWithFinite)
		{
			float fs[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
			auto frange = Range(fs);
			auto r = Zip(Iota(), frange);

			Assert::IsTrue(r.HasSize);
			Assert::AreEqual(frange.Size(), r.Size());

			int i = 0;
			for (; !r.IsEmpty(); r.Advance(), ++i)
			{
				std::tuple<size_t, float&> f = r.Front();
				Assert::AreEqual(size_t(i), std::get<0>(f));
				Assert::AreEqual(fs[i], std::get<1>(f));
			}
		}

		TEST_METHOD(ZipConstMutable)
		{
			int a[] = { 1,2,3,4 };
			const int b[] = { 5, 6, 7, 8 };

			auto r = Zip(Range(a), Range(b));
			Assert::IsTrue(std::is_same<std::tuple<int&, const int&>, decltype(r.Front())>::value);
		}

		TEST_METHOD(IterateRangeBased)
		{
			int arr[] = { 1, 2, 3, 4 };
			auto r = Range(arr);
			const auto cr = Range(arr);

			int i = 0;
			for (auto item : r)
			{
				Assert::AreEqual(arr[i], item);
				++i;
			}

			i = 0;
			for (auto item : cr)
			{
				Assert::AreEqual(arr[i], item);
				++i;
			}
		}
	};

	TEST_CLASS(TransformRangeTests)
	{
	public:
		TEST_METHOD(TransformLambda)
		{
			int arr[] = { 1, 2, 3, 4, 5 };
			auto r = Transform(Range(arr), [](int a) { return a * 5; });

			Assert::IsTrue(r.HasSize);
			Assert::AreEqual(size_t(5), r.Size());
			
			for (int i = 0; !r.IsEmpty(); ++i, r.Advance())
			{
				Assert::AreEqual(arr[i] * 5, r.Front());
			}
		}

		TEST_METHOD(TransformInfinite)
		{
			auto r = Transform(Iota<int>(), [](int a) { return a * 5; });

			Assert::IsFalse(r.HasSize);

			for (int i = 0; i < 10; ++i, r.Advance())
			{
				Assert::AreEqual(i * 5, r.Front());
			}
		}

		TEST_METHOD(TransformConstLambda)
		{
			const int arr[] = { 1, 2, 3, 4, 5 };
			auto r = Transform(Range(arr), [](int a) { return a * 5; });

			Assert::IsTrue(r.HasSize);
			Assert::AreEqual(size_t(5), r.Size());

			for (int i = 0; !r.IsEmpty(); ++i, r.Advance())
			{
				Assert::AreEqual(arr[i] * 5, r.Front());
			}
		}
	};

	TEST_CLASS(RangeWrapperTests)
	{
		TEST_METHOD(WrapPointerRange)
		{
			int is[] = { 1, 2, 3, 4, 5 };
			mu::ForwardRange<int> wrapped = WrapRange(Range(is));
			
			int index = 0;
			for (; !wrapped.IsEmpty(); wrapped.Advance(), ++index)
			{
				Assert::AreEqual(is[index], wrapped.Front());
			}
		}
	};
}
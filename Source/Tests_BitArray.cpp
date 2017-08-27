#include "CppUnitTest.h"
#include "mu-core/BitArray.h"
#include "mu-core/PrimitiveTypes.h"
#include "mu-core/Ranges.h"

#include "mu-core/mu-core-tests.inl"

namespace mu_core_tests_bitarray {
	using namespace mu;
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	using std::get;

	TEST_CLASS(BitArrayTests) {

		TEST_METHOD(InitBitsClear) {
			BitArray b;
			b.Init(256, false);

			for (size_t i = 0; i < 256; i += 7)
			{
				Assert::AreEqual(false, b.GetBit(i));
			}
		}
		TEST_METHOD(InitBitsSet) {
			BitArray b;
			b.Init(256, true);

			for (size_t i = 0; i < 256; i += 7)
			{
				Assert::AreEqual(true, b.GetBit(i));
			}
		}

		TEST_METHOD(SetBits) {
			BitArray b;
			b.Init(256, false);

			b.SetBit(12);
			b.SetBit(32);
			b.SetBit(64);
			b.SetBit(65);
			b.SetBit(100);

			Assert::AreEqual(true, b.GetBit(12));
			Assert::AreEqual(true, b.GetBit(32));
			Assert::AreEqual(true, b.GetBit(64));
			Assert::AreEqual(true, b.GetBit(65));
			Assert::AreEqual(true, b.GetBit(100));
		}

		TEST_METHOD(ClearBits) {
			BitArray b;
			b.Init(256, true);

			b.ClearBit(12);
			b.ClearBit(32);
			b.ClearBit(64);
			b.ClearBit(65);
			b.ClearBit(100);

			Assert::AreEqual(false, b.GetBit(12));
			Assert::AreEqual(false, b.GetBit(32));
			Assert::AreEqual(false, b.GetBit(64));
			Assert::AreEqual(false, b.GetBit(65));
			Assert::AreEqual(false, b.GetBit(100));
		}

		TEST_METHOD(IterateEmpty) {
			{
				BitArray b;
				auto r1 = b.GetSetBits();
				auto r2 = b.GetClearBits();
				Assert::IsTrue(r1.IsEmpty());
				Assert::IsTrue(r2.IsEmpty());
			}
			{
				BitArray b;
				b.Init(256, false);
				b.Empty();

				auto r1 = b.GetSetBits();
				auto r2 = b.GetClearBits();
				Assert::IsTrue(r1.IsEmpty());
				Assert::IsTrue(r2.IsEmpty());
			}
		}
		TEST_METHOD(IterateSetBits) {
			size_t indices[] = { 12, 32, 64, 65, 100 };

			BitArray b;
			b.Init(256, false);

			for (size_t i : indices) {
				b.SetBit(i);
			}

			for (auto pair : Zip(indices, b.GetSetBits())){
				Assert::AreEqual(get<0>(pair), get<1>(pair));
			}
		}
		TEST_METHOD(IterateClearBits) {}
	};
}
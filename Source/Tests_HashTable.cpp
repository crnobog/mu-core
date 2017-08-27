#include "CppUnitTest.h"
#include "mu-core/HashTable.h"

#include "mu-core/mu-core-tests.inl"

#include <random>

namespace mu_core_tests_hashtable {
	using namespace mu;
	using namespace Microsoft::VisualStudio::CppUnitTestFramework;

	struct Key {
		u64 Data;
	};
	struct Value {
		u64 Data;
	};

	bool operator==(const Key& a, const Key& b) {
		return a.Data == b.Data;
	}
	bool operator==(const Value& a, const Value& b) {
		return a.Data == b.Data;
	}
	
	TEST_CLASS(HashTableTests) {

		TEST_METHOD(ConstructSize) {
			HashTable<Key, Value> hash;
			Assert::AreEqual(0u, (u32)hash.GetNum());
		}

		TEST_METHOD(EmptyDoesNotContain) {
			Key k{ 17 };
			HashTable<Key, Value> hash;
			Assert::IsFalse(hash.Contains(k));
		}

		TEST_METHOD(AddContains) {
			Key k{ 17 };
			Value v{ 170 };
			HashTable<Key, Value> hash;
			hash.Add(k, v);
			Assert::AreEqual(1u, (u32)hash.GetNum());
			Assert::IsTrue(hash.Contains(k));
			Assert::IsTrue(hash.Find(k) == v);
		}

		TEST_METHOD(AddManyContains) {
			HashTable<Key, Value> hash;

			Array<Key> keys;
			Array<Value> values;

			std::mt19937 gen(23526234);
			i32 i = 0;
			while (keys.Num() < 1000) {
				Key k{ gen() };
				if (!keys.Contains(k)) {
					++i;
					keys.Emplace(k);
					values.Emplace(gen());
				}
			}

			for (i32 i = 0; i < keys.Num(); ++i) {
				hash.Add(keys[i], values[i]);
			}
			Assert::AreEqual(keys.Num(), hash.GetNum());

			for (Key k : keys) {
				bool hash_contains = hash.Contains(k);
				Assert::IsTrue(hash_contains);
			}
		}

		TEST_METHOD(IteratePairs) {
			HashTable<Key, Value> hash;

			Array<Key> keys;
			Array<Value> values;
			
			std::mt19937 gen(144155166);
			i32 i = 0;
			while(keys.Num() < 1000) {
				Key k{ gen() };
				if (!keys.Contains(k)) {
					++i;
					keys.Emplace(k);
					values.Emplace(gen());
				}
			}
			for (i32 i = 0; i < keys.Num(); ++i) {
				hash.Add(keys[i], values[i]);
			}

			Assert::AreEqual(keys.Num(), hash.GetNum());
			
			u64 expected_pairs = keys.Num();
			u64 num = 0;
			for (auto&[k, v] : Range(hash)) {
				++num;
				auto found_key = Find(keys, [k](const Key& arr_k) { return k == arr_k; });
				Assert::IsFalse(found_key.IsEmpty());
				size_t idx = &found_key.Front() - keys.Data();
				Assert::AreEqual(values[idx].Data, v.Data);
				values.RemoveAt(idx);
				keys.RemoveAt(idx);
			}

			Assert::IsTrue(keys.IsEmpty());
			Assert::IsTrue(values.IsEmpty());
			Assert::AreEqual(expected_pairs, num);
		}

		TEST_METHOD(AddRemove) {
			Key k{ 90 };
			Value v{ 170 };
			HashTable<Key, Value> hash;
			hash.Add(k, v);
			hash.Remove(k);
			Assert::AreEqual(0u, (u32)hash.GetNum());
			Assert::IsTrue(!hash.Contains(k));
		}

		TEST_METHOD(AddRemoveMany) {
			HashTable<Key, Value> hash;

			Array<Key> keys;
			Array<Value> values;

			std::mt19937 gen(3520389);
			i32 i = 0;
			while (keys.Num() < 1000) {
				Key k{ gen() };
				if (!keys.Contains(k)) {
					++i;
					keys.Emplace(k);
					values.Emplace(gen());
				}
			}
			for (i32 i = 0; i < keys.Num(); ++i) {
				hash.Add(keys[i], values[i]);
			}

			Assert::AreEqual(keys.Num(), hash.GetNum());
			Array<Key> shuffled_keys;
			while (keys.Num()) {
				size_t i = gen() % keys.Num();
				shuffled_keys.Add(keys[i]);
				keys.RemoveAt(i);
			}

			for (Key k : shuffled_keys) {
				hash.Remove(k);
			}
			Assert::AreEqual(0ull, hash.GetNum());
		}
	};
}
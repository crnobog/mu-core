static void BM_HashTable_Fill_Dense(benchmark::State& st) {
	while (st.KeepRunning()) {
		mu::HashTable<i32, i32> h;
		i32 num = st.range(0);
		for (i32 i = 0; i < num; ++i) {
			h.Add({ i }, { i });
		}
	}
}
BENCHMARK(BM_HashTable_Fill_Dense)->Range(32, 4 << 10);

#include <unordered_map>

static void BM_unorderedmap_Fill_Dense(benchmark::State& st) {
	while (st.KeepRunning()) {
		std::unordered_map<i32, i32> m; 
		i32 num = st.range(0);
		for (i32 i = 0; i < num; ++i) {
			m.insert({ i, i });
		}
	}
}
BENCHMARK(BM_unorderedmap_Fill_Dense)->Range(32, 4 << 10);
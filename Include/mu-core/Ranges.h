#pragma once

#include "mu-core/Functors.h"
#include "mu-core/Metaprogramming.h"

#include "mu-core/IotaRange.h"
#include "mu-core/PointerRange.h"
#include "mu-core/TransformRange.h"
#include "mu-core/ZipRange.h"

#include "mu-core/RangeIteration.h"

#include <tuple>

// Prototype of a forward range:
//	template<typename T>
//	class ForwardRange
//	{
//		enum {HasSize = ?};
//
//		void Advance();
//		bool IsEmpty();
//		T& Front();
//		size_t Size(); // if HasSize == 1
//	};

template<typename T>
class Array;


namespace mu {
	template<typename T, size_t MAX>
	class FixedArray;

	// Functions to automatically construct ranges from pointers/arrays
	template<typename T>
	auto Range(T* ptr, size_t num) {
		return Range(ptr, ptr + num);
	}

	template<typename T>
	auto Range(T* start, T* end) {
		return PointerRange<T>(start, end);
	}

	template<typename T, size_t SIZE>
	auto Range(T(&arr)[SIZE]) {
		return Range(arr, arr + SIZE);
	}

	template<typename T>
	auto Range(Array<T>& arr) {
		return Range(arr.Data(), arr.Num());
	}

	template<typename T>
	auto Range(const Array<T>& arr) {
		return Range(arr.Data(), arr.Num());
	}

	template<typename T, size_t MAX>
	auto Range(FixedArray<T, MAX>& arr) {
		return Range(arr.Data(), arr.Num());
	}

	template<typename T, size_t MAX>
	auto Range(const FixedArray<T, MAX>& arr) {
		return Range(arr.Data(), arr.Num());
	}

	template<typename RANGE>
	auto Range(RANGE&& r) {
		return std::forward<RANGE>(r);
	}

	template<typename... RANGES>
	auto Zip(RANGES&&... ranges) {
		return ZipRange<decltype(Range(std::forward<RANGES>(ranges)))...>(Range(std::forward<RANGES>(ranges))...);
	}

	template<typename T = size_t>
	inline auto Iota(T start = 0) { return IotaRange<T>(start); }

	template<typename IN_RANGE, typename FUNC>
	auto Transform(IN_RANGE&& r, FUNC&& f) {
		return TransformRange<IN_RANGE, FUNC>(
			std::forward<std::decay<IN_RANGE>::type>(r),
			std::forward<std::decay<FUNC>::type>(f));
	}

	template<typename R>
	auto MakeRangeIterator(R&& r) {
		typedef std::decay<R>::type RANGE_TYPE;
		return details::RangeIterator<RANGE_TYPE>(std::forward<RANGE_TYPE>(r));
	}

	inline auto MakeRangeSentinel() {
		return details::RangeSentinel{};
	}
}
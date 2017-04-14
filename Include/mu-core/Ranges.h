#pragma once

#include <tuple>
#include <limits>
#include <memory>

#include "Functors.h"

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

	using mu::functor::Fold;
	using mu::functor::FoldOr;
	using mu::functor::FMap;
	using mu::functor::FMapVoid;

	namespace details {
		template<typename T> struct WithBeginEnd;
	}

	// A linear forward range over raw memory of a certain type
	template<typename T>
	class PointerRange : public details::WithBeginEnd<PointerRange<T>> {
		T* m_start, *m_end;

	public:
		static constexpr bool HasSize = true;

		template<class U>
		friend class PointerRange;

		PointerRange()
			: m_start(nullptr)
			, m_end(nullptr) {}

		PointerRange(T* start, T* end)
			: m_start(start)
			, m_end(end) {}

		template<typename U>
		PointerRange(PointerRange<U> other) {
			m_start = other.m_start;
			m_end = other.m_end;
		}

		void Advance() { ++m_start; }
		void AdvanceBy(size_t num) { m_start += num; }

		bool IsEmpty() const { return m_start >= m_end; }
		T& Front() { return *m_start; }
		const T& Front() const { return *m_start; }
		size_t Size() const { return m_end - m_start; }

		template<typename U>
		bool operator==(const PointerRange<U>& other) const {
			return m_start == other.m_start
				&& m_end == other.m_end;
		}

		template<typename U>
		bool operator!=(const PointerRange<U>& other) const {
			return m_start != other.m_start
				|| m_end != other.m_end;
		}

		PointerRange MakeEmpty() const { return PointerRange{ nullptr, nullptr }; }
	};

	// A linear infinite range over an integral type
	template<typename T>
	class IotaRange {
		T m_it = 0;
	public:
		enum { HasSize = 0 };

		IotaRange() {}
		IotaRange(T start = 0) : m_it(start) {}

		void Advance() { ++m_it; }
		bool IsEmpty() const { return false; }
		T Front() { return m_it; }
	};

	// ZipRange combines multiple ranges and iterates them in lockstep
	template<typename... RANGES>
	class ZipRange : public details::WithBeginEnd<ZipRange<RANGES...>> {
		std::tuple<RANGES...> m_ranges;

		template<size_t... INDICES>
		ZipRange MakeEmpty(std::index_sequence<INDICES...>) const {
			return ZipRange{ std::get<INDICES>(m_ranges).MakeEmpty()... };
		}

	public:
		static constexpr bool HasSize = FoldOr(RANGES::HasSize...);

		ZipRange(RANGES... ranges) : m_ranges(ranges...) {}

		bool IsEmpty() const {
			return FoldOr(FMap<details::RangeIsEmpty>(m_ranges));
		}

		void Advance() {
			FMapVoid<details::RangeAdvance>(m_ranges);
		}

		auto Front() {
			return FMap<details::RangeFront>(m_ranges);
		}

		template<typename T = ZipRange, typename std::enable_if<T::HasSize, int>::type = 0>
		size_t Size() const {
			return Fold<details::RangeMinSizeFolder>(
				std::numeric_limits<size_t>::max(), m_ranges);
		}

		ZipRange MakeEmpty() const { return MakeEmpty(mu::functor::details::TupleIndices<decltype(m_ranges)>()); }
	};

	template<typename IN_RANGE, typename FUNC>
	class TransformRange : public details::WithBeginEnd<TransformRange<IN_RANGE, FUNC>> {
		IN_RANGE m_range;
		FUNC m_func;
	public:
		static constexpr bool HasSize = IN_RANGE::HasSize;

		TransformRange(IN_RANGE r, FUNC f)
			: m_range(std::move(r)), m_func(std::move(f)) {}

		bool IsEmpty() const { return m_range.IsEmpty(); }
		auto Front() { return m_func(m_range.Front()); }
		void Advance() { m_range.Advance(); }

		template<typename T = IN_RANGE, typename = std::enable_if_t<T::HasSize>>
		size_t Size() const { return m_range.Size(); }

		TransformRange MakeEmpty() const { return TransformRange{ m_range.MakeEmpty(), m_func }; }
	};


	template<typename ELEMENT>
	class ForwardRange {
		struct WrapperBase {
			virtual bool IsEmpty() = 0;
			virtual void Advance() = 0;
			virtual ELEMENT Front() = 0;
		};

		template<typename RANGE>
		class Wrapper : public WrapperBase {
			RANGE m_range;
		public:
			Wrapper(RANGE in_r)
				: m_range(std::move(in_r)) {}

			virtual bool IsEmpty() { return m_range.IsEmpty(); }
			virtual void Advance() { m_range.Advance(); }
			virtual ELEMENT Front() { return m_range.Front(); }
		};

		std::unique_ptr<WrapperBase> m_wrapper;
	public:

		template<typename RANGE>
		ForwardRange(RANGE&& in_r) {
			m_wrapper = std::make_unique<Wrapper<RANGE>>(std::forward<RANGE>(in_r));
		}

		bool IsEmpty() { return m_wrapper->IsEmpty(); }
		void Advance() { m_wrapper->Advance(); }
		ELEMENT Front() { return m_wrapper->Front(); }
	};

	namespace details {
		template<typename RANGE>
		struct WithBeginEnd {
			auto begin() const { return RangeIterator<RANGE>{ *static_cast<const RANGE*>(this)}; }
			auto end() const { return RangeIterator<RANGE>{ static_cast<const RANGE*>(this)->MakeEmpty() }; }
		};

		// Helpers for calling members in variadic template expansion
		template<typename RANGE>
		struct RangeIsEmpty { bool operator()(const RANGE& r) { return r.IsEmpty(); } };

		template<typename RANGE>
		struct RangeAdvance { void operator()(RANGE& r) { r.Advance(); } };

		template<typename RANGE>
		struct RangeFront {
			auto operator()(RANGE& r) -> decltype(r.Front()) {
				return r.Front();
			}
		};

		// Functor for folding over ranges of finite/infinite size and picking the minimum size
		template<typename RANGE>
		struct RangeMinSizeFolder {
			template<typename T = RANGE, typename std::enable_if<T::HasSize, int>::type = 0>
			size_t operator()(size_t s, const RANGE& r) const {
				size_t rs = r.Size();
				return rs < s ? rs : s;
			}

			template<typename T = RANGE, typename std::enable_if<!T::HasSize, int>::type = 0>
			size_t operator()(size_t s, const RANGE&) const {
				return s;
			}
		};

		template<typename RANGE>
		using RangeFrontType = decltype(std::declval<RANGE>().Front());

		// Adaptor for using ranges in begin-end based range-based for loops
		template<typename RANGE>
		struct RangeIterator {
			RANGE m_range;

			RangeIterator(RANGE r) : m_range(std::move(r)) {}

			void operator++() { m_range.Advance(); }
			RangeFrontType<RANGE> operator*() { return m_range.Front(); }
			bool operator!=(const RangeIterator&) { return !m_range.IsEmpty(); }
		};
	}

	template<typename R>
	auto MakeRangeIterator(R&& r) {
		typedef std::decay<R>::type RANGE_TYPE;
		return details::RangeIterator<RANGE_TYPE>(std::forward<RANGE_TYPE>(r));
	}

	template<typename RANGE>
	auto WrapRange(RANGE&& in_r) {
		typedef decltype(Range(in_r).Front()) ElementType;
		return ForwardRange<ElementType>(Range(std::forward<RANGE>(in_r)));
	}
}
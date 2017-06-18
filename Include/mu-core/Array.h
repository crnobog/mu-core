#pragma once

#include <initializer_list>

#include "mu-core/PointerRange.h"
#include "mu-core/Algorithms.h"

namespace mu {
	template<typename T>
	class Array {
		T* m_data = nullptr;
		size_t m_num = 0;
		size_t m_max = 0;

	public:
		Array() {}

		Array(std::initializer_list<T> init) {
			InitEmpty(init.size());
			for (auto&& item : init) {
				new(&AddSafe()) T(item);
			}
		}

		template<class RANGE>
		Array(RANGE&& r) {
			for (auto&& item : r) {
				Add(item);
			}
		}

		Array(const Array& other) {
			InitEmpty(other.m_num);
			for (auto&& item : other) {
				Add(item);
			}
		}

		Array(Array&& other) {
			*this = std::forward<Array>(other);
		}

		Array& operator=(const Array& other) {
			InitEmpty(other.Num());
			for (const auto& item : other) {
				Add(item);
			}
			return *this;
		}

		Array& operator=(Array&& other) {
			this->~Array();

			std::swap(m_data, other.m_data);
			std::swap(m_num, other.m_num);
			std::swap(m_max, other.m_max);
			return *this;
		}

		~Array() {
			Destruct(0, m_num);
			if (m_data) { free(m_data); }
		}

		void Reserve(size_t new_max) {
			if (new_max > m_max) {
				Grow(new_max);
			}
		}

		static Array MakeUninitialized(size_t num) {
			Array ret{};
			ret.m_data = (T*)malloc(sizeof(T)* num);
			ret.m_num = num;
			ret.m_max = num;
			return std::move(ret);
		}

		template<typename... US>
		static Array Make(US&&... us) {
			Array ret{};
			ret.Reserve(sizeof...(US));
			ret.EmplaceMany(std::forward<US>(us)...);
			return std::move(ret);
		}

		template<typename... US>
		static Array MakeUnique(US&&... us) {
			Array ret{};
			ret.Reserve(sizeof...(US));
			ret.AddManyUnique(std::forward<US>(us)...);
			return std::move(ret);
		}

		size_t Add(const T& item) {
			EnsureSpace(m_num + 1);
			new(&AddSafe()) T(item);
			return m_num - 1;
		}

		size_t Add(T&& item) {
			EnsureSpace(m_num + 1);
			return EmplaceSafe(std::forward<T>(item));
		}

		void AddZeroed(size_t count) {
			EnsureSpace(m_num + count);
			for (size_t i = 0; i < count; ++i) {
				T& item = AddSafe();
				memset(&item, 0, sizeof(T));
			}
		}

		void AddUnique(const T& item) {
			if (!Contains(item)) {
				Add(item);
			}
		}

		void AddUnique(T&& item) {
			if (!Contains(item)) {
				Add(std::forward<T>(item));
			}
		}

		template<typename U>
		void AddManyUnique(U&& u) {
			AddUnique(std::forward<U>(u));
		}

		template<typename U, typename... US>
		void AddManyUnique(U&& u, US&&... us) {
			AddUnique(std::forward<U>(u));
			AddManyUnique(std::forward<US>(us)...);
		}

		template<typename U>
		void AddMany(U&& u) {
			Add(std::forward<U>(u));
		}

		template<typename U, typename... US>
		void AddMany(U&& u, US&&... us) {
			Add(std::forward<U>(u));
			AddMany(std::forward<US>(us)...);
		}

		template<typename U>
		void EmplaceMany(U&& u) {
			Emplace(std::forward<U>(u));
		}

		template<typename U, typename... US>
		void EmplaceMany(U&& u, US&&... us) {
			Emplace(std::forward<U>(u));
			EmplaceMany(std::forward<US>(us)...);
		}

		size_t Emplace(T&& item) {
			EnsureSpace(m_num + 1);
			return EmplaceSafe(std::forward<T>(item));
		}

		template<typename... US>
		size_t Emplace(US&&... us) {
			return Add(T(std::forward<US>(us)...));
		}

		template<typename RANGE>
		void Append(RANGE&& r) {
			for (auto&& item : r) {
				Add(std::forward<decltype(item)>(item));
			}
		}

		void AppendRaw(const T* items, size_t count) {
			Append(mu::Range(items, count));
		}

		void RemoveAt(size_t index) {
			mu::Move(mu::Range(m_data + index, m_num - index),
				mu::Range(m_data + index + 1, m_num - index - 1));
			m_num -= 1;
		}

		T& operator[](size_t index) {
			return m_data[index];
		}

		const T& operator[](size_t index) const {
			return m_data[index];
		}

		T* Data() { return m_data; }
		const T* Data() const { return m_data; }

		size_t Num() const { return m_num; }
		size_t Max() const { return m_max; }
		bool IsEmpty() const { return m_num == 0; }

		// TODO: Remove in favour of algorithms?
		bool Contains(const T& item) const {
			for (const T& t : *this) {
				if (t == item) {
					return true;
				}
			}
			return false;
		}

		auto begin() { return mu::MakeRangeIterator(mu::Range(m_data, m_num)); }
		auto end() { return mu::MakeRangeSentinel(); }

		auto begin() const { return mu::MakeRangeIterator(mu::Range(m_data, m_num)); }
		auto end() const { return mu::MakeRangeSentinel(); }

	private:
		void InitEmpty(size_t num) {
			m_data = (T*)malloc(sizeof(T) * num);
			m_num = 0;
			m_max = num;
		}

		void EnsureSpace(size_t num) {
			if (num > m_max) {
				Grow(num > m_max * 2 ? num : m_max * 2);
			}
		}

		void Grow(size_t new_size) {
			T* new_data = (T*)malloc(sizeof(T) * new_size);
			auto from = mu::Range(m_data, m_num);
			auto to = mu::Range(new_data, m_num);
			mu::MoveConstruct(to, from);
			m_data = new_data;
			m_max = new_size;
		}

		T& AddSafe() {
			T* item = m_data + m_num++;
			return *item;
		}

		size_t EmplaceSafe(T&& item) {
			new(m_data + m_num) T(std::forward<T>(item));
			return m_num++;
		}

		void Destruct(size_t start, size_t num) {
			for (size_t i = start; i < start + num; ++i) {
				m_data[i].~T();
			}
		}
	};
}
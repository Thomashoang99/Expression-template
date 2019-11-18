#pragma once
#include "Impl.hpp"
#include <vector>

namespace et
{
	using namespace et_impl;

	template<typename T, typename Allocator = std::allocator<T>>
	class et_vector : public Expression<T, et_vector<T, Allocator>>
	{
		std::vector<T, Allocator> elems{};

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = std::size_t;
		using reference = T&;
		using const_reference = const T&;
		using iterator = typename decltype(elems)::iterator;
		using const_iterator = typename decltype(elems)::const_iterator;
		using reverse_iterator = typename std::reverse_iterator<iterator>;
		using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

	public:
		et_vector() = default;

		et_vector(std::size_t n, T&& init = T()) : elems(n, init)
		{}

		et_vector(std::initializer_list<T> ilist)
		{
			elems.assign(ilist);
		}

		et_vector(const et_vector<T>& rhs) :
			elems(rhs.cbegin(), rhs.cend())
		{}

		et_vector(et_vector<T>&& rhs) : et_vector()
		{
			this->swap(rhs);
		}

		template<typename E>
		et_vector(const Expression<T, E>& e)
		{
			et_vector tmp(e.size());
			for (std::size_t i = 0, s = e.size(); i < s; ++i)
				tmp[i] = e[i];
			this->swap(tmp);
		}

		et_vector& operator=(const et_vector& rhs) = delete;
		et_vector& operator=(et_vector&& rhs) = delete;

		template<typename E>
		et_vector& operator=(const Expression<T, E>& e)
		{
			et_vector tmp(e.size());
			for (std::size_t i = 0, s = e.size(); i < s; ++i)
				tmp[i] = e[i];
			this->swap(tmp);
			return *this;
		}


		~et_vector() {}

	public:
		decltype(auto) operator[](std::size_t i) const noexcept
		{
			return elems[i];
		}

		decltype(auto) operator[](std::size_t i) noexcept
		{
			return elems[i];
		}

		size_type size() const noexcept
		{
			return elems.size();
		}


	public:
		void insert(T&& value)
		{
			elems.push_back(std::move(value));
		}

		template<typename...Args>
		void emplace(Args&&...args)
		{
			elems.emplace_back(std::forward<Args>(args)...);
		}

		void swap(et_vector<T>& rhs) noexcept
		{
			elems.swap(rhs.elems);
		}

	public:
		decltype(auto) begin()	noexcept { return elems.begin(); }
		decltype(auto) end()	noexcept { return elems.end(); }
		decltype(auto) rbegin() noexcept { return elems.rbegin(); }
		decltype(auto) rend()	noexcept { return elems.rend(); }
		decltype(auto) cbegin() const noexcept { return elems.cbegin(); }
		decltype(auto) cend()	const noexcept { return elems.cend(); }
		decltype(auto) crbegin()const noexcept { return elems.crbegin(); }
		decltype(auto) crend()	const noexcept { return elems.crend(); }
	};
};


namespace lazy
{
	using namespace et_operators;
	using namespace et_impl;

	template<typename T, typename E, typename Op>
	decltype(auto) evaluate(const VecReduction<T, E, Op>& f)
	{
		return f.eval();
	}

	template<typename T, typename E1, typename E2>
	decltype(auto) dot_product(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
	{
		static_assert(std::is_arithmetic_v<T>);
		const auto& mul_result = lhs * rhs;
		return VecReduction<T, decltype(mul_result), std::plus<>>
			(mul_result, std::plus<>());
	}
};
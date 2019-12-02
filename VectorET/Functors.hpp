#pragma once

namespace unary_functors
{
	template<typename Constant>
	struct unary_mul
	{
		Constant c;
		constexpr unary_mul(const Constant& _c) : c(_c) {}

		template<typename T>
		constexpr auto operator()(T&& value) const
			noexcept(noexcept(static_cast<T&&>(value)* c))
		{
			return static_cast<T&&>(value)* c;
		}
	};

	template<typename Constant>
	struct unary_div
	{
		Constant c;
		constexpr unary_div(const Constant& _c) : c(_c) {}

		template<typename T>
		constexpr auto operator()(T&& value) const
			noexcept(noexcept(static_cast<T&&>(value) / c))
		{
			return static_cast<T&&>(value) / c;
		}
	};

}
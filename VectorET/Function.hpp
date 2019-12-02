#pragma once
#include "Impl.hpp"
#include "Operators.hpp"


namespace lazy_functions
{
	using namespace operators;
	using namespace impl;

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
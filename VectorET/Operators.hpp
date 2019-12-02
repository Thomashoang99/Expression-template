#pragma once
#include "Impl.hpp"
#include "Functors.hpp"

namespace operators
{
	using namespace impl;
	using namespace unary_functors;


	template<typename T, typename E1, typename E2>
	auto operator+(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
	{
		return VecOpVec<T, E1, E2, std::plus<>>
			(*static_cast<const E1*>(&lhs), std::plus<>(), *static_cast<const E2*>(&rhs));
	}

	template<typename T, typename E1, typename E2>
	auto operator-(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
	{
		return VecOpVec<T, E1, E2, std::minus<>>
			(*static_cast<const E1*>(&lhs), std::minus<>(), *static_cast<const E2*>(&rhs));
	}

	template<typename T, typename E1, typename E2>
	auto operator*(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
	{
		return VecOpVec<T, E1, E2, std::multiplies<>>
			(*static_cast<const E1*>(&lhs), std::multiplies<>(), *static_cast<const E2*>(&rhs));
	}

	template<typename T, typename E1, typename E2>
	auto operator/(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
	{
		return VecOpVec<T, E1, E2, std::divides<>>
			(*static_cast<const E1*>(&lhs), std::divides<>(), *static_cast<const E2*>(&rhs));
	}

	template<typename T, typename E>
	auto operator*(const T& scalar, const Expression<T, E>& e)
	{
		return VecTransform<T, E, unary_mul<T>>
			(*static_cast<const E*>(&e), unary_mul<T>(scalar));
	}

	template<typename T, typename E>
	auto operator*(const Expression<T, E>& e, const T& scalar)
	{
		return VecTransform<T, E, unary_mul<T>>
			(*static_cast<const E*>(&e), unary_mul<T>(scalar));
	}

	template<typename T, typename E>
	auto operator/(const Expression<T, E>& e, const T& scalar)
	{
		return VecTransform<T, E, unary_div<T>>
			(*static_cast<const E*>(&e), unary_div<T>(scalar));
	}
};
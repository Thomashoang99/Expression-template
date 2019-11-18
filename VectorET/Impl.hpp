#pragma once
#include <cassert>
#include <functional>

namespace et_impl
{

	template<typename T, typename E>
	class Expression
	{
	public:
		decltype(auto) operator[](std::size_t i) const {
			return static_cast<const E&>(*this)[i];
		}
		size_t size() const {
			return std::size(static_cast<const E&>(*this));
		}
	};


	template<typename T, typename E1, typename E2, typename BinaryOp>
	class VecOpVec : public Expression<T, VecOpVec<T, E1, E2, BinaryOp>>
	{
		E1 const& lhs;
		BinaryOp op;
		E2 const& rhs;
	public:
		VecOpVec(const E1& l, BinaryOp o, const E2& r) : lhs(l), op(o), rhs(r)
		{
			assert(l.size() == r.size());
		}
		~VecOpVec() {}
	public:
		decltype(auto) operator[](std::size_t i) const
		{
			return op(lhs[i], rhs[i]);
		}
		auto size() const
		{
			return rhs.size();
		}
	};


	template<typename T, typename E, typename BinaryOp>
	class ScalarOpVec : public Expression<T, ScalarOpVec<T, E, BinaryOp>>
	{
		const T& scalar;
		BinaryOp op;
		const E& expr;
	public:
		ScalarOpVec(const T& s, BinaryOp o, const E& e) : scalar(s), op(o), expr(e) {}
		~ScalarOpVec() {}
	public:
		decltype(auto) operator[](std::size_t i) const { return op(scalar, expr[i]); }
		auto size() const { return expr.size(); }
	};


	template<typename T, typename E, typename BinaryOp>
	class VecOpScalar : public Expression<T, VecOpScalar<T, E, BinaryOp>>
	{
		const E& expr;
		BinaryOp op;
		const T& scalar;
	public:
		VecOpScalar(const E& e, BinaryOp o, const T& s) : scalar(s), op(o), expr(e) {}
		~VecOpScalar() {}
	public:
		decltype(auto) operator[](std::size_t i) const { return op(expr[i], scalar); }
		auto size() const { return expr.size(); }
	};


	template<typename T, typename E, typename Op>
	class VecReduction : private Expression<T, VecReduction<T, E, Op>>
	{
		const E& expr;
		Op op;
	public:
		VecReduction(const E& e, const Op& op) : expr(e), op(op)
		{}
		~VecReduction() {}
	private:
		auto size() const { return expr.size(); }
	public:
		auto eval() const
		{
			T&& init = T();
			for (std::size_t i = 0, sz = expr.size(); i < sz; ++i)
				init = op(std::move(init), expr[i]);
			return init;
		}
	};
};


namespace et_operators
{
	using namespace et_impl;

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


	template<typename T, typename E>
	auto operator*(const T& scalar, const Expression<T, E>& v)
	{
		return ScalarOpVec<T, E, std::multiplies<>>
			(scalar, std::multiplies<>(), *static_cast<const E*>(&v));
	}

	template<typename T, typename E>
	auto operator*(const Expression<T, E>& v, const T& scalar)
	{
		return VecOpScalar<T, E, std::multiplies<>>
			(*static_cast<const E*>(&v), std::multiplies<>(), scalar);
	}

	template<typename T, typename E>
	auto operator/(const Expression<T, E>& v, const T& scalar)
	{
		return VecOpScalar<T, E, std::divides<>>
			(*static_cast<const E*>(&v), std::divides<>(), scalar);
	}
};
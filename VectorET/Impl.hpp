#pragma once
#include <cassert>
#include <functional>

namespace impl
{
	template<typename T, typename E>
	class Expression
	{
	public:

		decltype(auto) operator[](std::size_t i) const {
			return static_cast<const E&>(*this)[i];
		}
		std::size_t size() const {
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
		{}
		~VecOpVec() {}
	public:
		decltype(auto) operator[](std::size_t i) const
		{
			return op(lhs[i], rhs[i]);
		}
		std::size_t size() const
		{
			return rhs.size();
		}
	};


	template<typename T, typename E, typename Op>
	class VecReduction : public Expression<T, VecReduction<T, E, Op>>
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
		auto operator[](std::size_t i) const
		{
			T&& init = T();
			for (std::size_t j = 0; j < i; ++j)
				init = op(std::move(init), expr[j]);
			return init;
		}
		auto eval() const
		{
			return this->operator[](size() - 1);
		}
	};


	template<typename T, typename E, typename Op>
	class VecTransform : public Expression<T, VecTransform<T, E, Op>>
	{
		const E& expr;
		Op op;
	public:
		VecTransform(const E& e, const Op& op) : expr(e), op(op)
		{}
		~VecTransform() {}
	public:
		auto size() const { return expr.size(); }
		decltype(auto) operator[](std::size_t i) const
		{
			return op(expr[i]);
		}
	};

	template<typename T, typename E1, typename E2>
	class VecConcat : public Expression<T, VecConcat<T, E1, E2>>
	{
		const E1& lhs;
		const E2& rhs;
	public:
		VecConcat(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs){}
		~VecConcat(){}
	public:
		auto size() const { return lhs.size() + rhs.size(); }
		decltype(auto) operator[](std::size_t i) const
		{
			auto lsz = lhs.size();
			return i >= lsz ? i : i - lsz;
		}
	};
};



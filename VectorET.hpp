#pragma once
#include <vector>
#include <functional>
#include <cassert>

//Base vector-expression
template<typename T, typename E>
class Expression
{
public:
	const T& operator[](std::size_t i) const { return static_cast<const E&>(*this)[i]; }
	size_t size() const { return std::size(static_cast<const E&>(*this)); }
};


//Vector, CRTP inheritance from base
template<typename T>
class Vector : public Expression<T, Vector<T>>
{
	std::vector<T> elems{};
public:
	Vector() = default;
	Vector(std::size_t n) : elems(n) {}

	template<typename E>
	Vector(const Expression<T, E>& e) : elems(e.size())
	{
		for (std::size_t i = 0, s = size(); i < s; ++i)
			elems[i] = e[i];
	}

	template<typename E>
	Vector& operator=(const Expression<T, E>& e)
	{
		for (std::size_t i = 0, s = size(); i < s; ++i)
			elems[i] = e[i];
		return *this;
	}



	Vector(std::initializer_list<T> ilist) : elems(ilist.begin(), ilist.end())
	{
	}
	~Vector() {}

public:
	const T& operator[](std::size_t i) const { return elems[i]; }
	T& operator[](std::size_t i) { return elems[i]; }
	auto size() const { return elems.size(); }

public:
	decltype(auto) begin() { return elems.begin(); }
	decltype(auto) end() { return elems.end(); }
	decltype(auto) rbegin() { return elems.rbegin(); }
	decltype(auto) rend() { return elems.rend(); }
};



//Expression evaluation of type <V op V>
template<typename T, typename E1, typename E2, typename BinaryOp>
class VecOpVec : public Expression<T, VecOpVec<T, E1, E2, BinaryOp>>
{
	E1 const& lhs;
	BinaryOp op;
	E2 const& rhs;
public:
	VecOpVec(const E1& l, BinaryOp o, const E2& r) : lhs(l), op(o), rhs(r) {
		assert(lhs.size() == rhs.size());
	}
	~VecOpVec() {}
public:
	const T& operator[](std::size_t i) const { return op(lhs[i], rhs[i]); }
	auto size() const { return rhs.size(); }
};

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




///------------------------------------------------------------------
//Expression evaluation of type <S op V>
template<typename T, typename E, typename UnaryOp>
class ScalarOpVec : public Expression<T, ScalarOpVec<T, E, UnaryOp>>
{
	T scalar;
	UnaryOp op;
	const E& expr;
public:
	ScalarOpVec(const T& s, UnaryOp o, const E& e) : scalar(s), op(o), expr(e) {}
	~ScalarOpVec(){}
public:
	const T& operator[](std::size_t i) const { return op(scalar, expr[i]); }
	auto size() const { return expr.size(); }
};


template<typename T, typename E>
auto operator*(const T& scalar, const Expression<T, E>& v)
{
	return ScalarOpVec<T, E, std::multiplies<>>
		(scalar, std::multiplies<>(), *static_cast<const E*>(&v));
}




///--------------------------------------------------
//Expression evaluation of type <V op S>
template<typename T, typename E, typename UnaryOp>
class VecOpScalar : public Expression<T, VecOpScalar<T, E, UnaryOp>>
{
	const E& expr;
	UnaryOp op;
	T scalar;
public:
	VecOpScalar(const E& e, UnaryOp o, const T& s) : scalar(s), op(o), expr(e) {}
	~VecOpScalar() {}
public:
	const T& operator[](std::size_t i) const { return op(expr[i], scalar); }
	auto size() const { return expr.size(); }

};


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
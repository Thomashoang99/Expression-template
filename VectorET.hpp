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



//Expression evaluation with generic binary operation
//CRTP inheritance
template<typename T, typename E1, typename E2, typename BinaryOp>
class BinaryVecOp : public Expression <T, BinaryVecOp<T, E1, E2, BinaryOp>>
{
	E1 const& lhs;
	E2 const& rhs;
	BinaryOp op;
public:
	BinaryVecOp(const E1& l, const E2& r, BinaryOp o) : lhs(l), rhs(r), op(o) {
		assert(l.size() == r.size());
	}
	~BinaryVecOp() {}
public:
	const T& operator[](std::size_t i) const { return op(lhs[i], rhs[i]); }
	auto size() const { return rhs.size(); }
};


template<typename T, typename E1, typename E2>
auto operator+(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
{
	return BinaryVecOp<T, E1, E2, std::plus<>>
		(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs),
			std::plus<>());
}

template<typename T, typename E1, typename E2>
auto operator-(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
{
	return BinaryVecOp<T, E1, E2, std::minus<>>
		(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs),
			std::minus<>());
}

template<typename T, typename E1, typename E2>
auto operator*(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
{
	return BinaryVecOp<T, E1, E2, std::multiplies<>>
		(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs),
			std::multiplies<>());
}

template<typename T, typename E1, typename E2>
auto operator/(const Expression<T, E1>& lhs, const Expression<T, E2>& rhs)
{
	return BinaryVecOp<T, E1, E2, std::divides<>>
		(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs),
			std::divides<>());
}
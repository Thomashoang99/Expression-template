#include <iostream>
#include "Vector.hpp"
#include "Operators.hpp"

using operators::operator*, operators::operator/;

int main()
{
	//std::ios_base::sync_with_stdio(false);
	et::et_vector a{ 1, 9, -8, 5, 2, 9, 0 };
	a = (3 * a) / 2;
	for (auto& n : a)
		std::cout << n << ", ";
	std::cin.get();
}
#include <vector>
#include "vector.hpp"
#include <iostream>

int main()
{

	// std::vector<int> input_vector(10, 1);
	// std::vector<int> output_vector(30);

	// output_vector.insert(output_vector.begin(), 10, 5);

	// std::vector<int>::iterator first = input_vector.begin();
	// std::vector<int>::iterator last = input_vector.end();

	// for (std::vector<int>::iterator it = output_vector.begin(); it != output_vector.end(); it++)
	// 	std::cout << *it << std::endl;

	// std::cout << std::endl;

	// output_vector.insert(output_vector.begin(), last, first);

	// for (std::vector<int>::iterator it = output_vector.begin(); it != output_vector.end(); it++)
	// 	std::cout << *it << std::endl;

	ft::vector<int> v;

	std::cout << "[";
	for (ft::vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		std::cout << " " << *it;
		if (it != v.end() - 1)
			std::cout << ",";
	}
	std::cout << " ]" << std::endl;
}
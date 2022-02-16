#include <vector>
#include <list>
#include "vector.hpp"
#include <iostream>

#define TESTED_NAMESPACE ft

#define T_SIZE_TYPE typename TESTED_NAMESPACE::vector<T>::size_type

template <typename T>
void	printSize(TESTED_NAMESPACE::vector<T> const &vct, bool print_content = true)
{
	const T_SIZE_TYPE size = vct.size();
	const T_SIZE_TYPE capacity = vct.capacity();
	const std::string isCapacityOk = (capacity >= size) ? "OK" : "KO";
	// Cannot limit capacity's max value because it's implementation dependent

	std::cout << "size: " << size << std::endl;
	std::cout << "capacity: " << isCapacityOk << std::endl;
	std::cout << "max_size: " << vct.max_size() << std::endl;
	if (print_content)
	{
		typename TESTED_NAMESPACE::vector<T>::const_iterator it = vct.begin();
		typename TESTED_NAMESPACE::vector<T>::const_iterator ite = vct.end();
		std::cout << std::endl << "Content is:" << std::endl;
		for (; it != ite; ++it)
			std::cout << "- " << *it << std::endl;
	}
	std::cout << "###############################################" << std::endl;
}

int main()
{
	std::list<int> lst;
	std::list<int>::iterator lst_it;
	for (int i = 1; i < 5; ++i)
		lst.push_back(i * 3);

	TESTED_NAMESPACE::vector<int> vct(lst.begin(), lst.end());
	printSize(vct);

	lst_it = lst.begin();
	for (int i = 1; lst_it != lst.end(); ++i)
		*lst_it++ = i * 5;
	vct.assign(lst.begin(), lst.end());
	printSize(vct);

	vct.insert(vct.end(), lst.rbegin(), lst.rend());
	std::cout << std::endl;
	printSize(vct);

	return (0);

	// std::vector<int> v;

	// std::cout << "size: " << v.size() << " - capacity: " << v.capacity() << std::endl;
	// std::cout << "v.end(): " << &(*v.end()) << " - v.begin(): " << &(*v.begin()) << std::endl;

	// v.reserve(4);

	// std::cout << "v.end() after reserve: " << &(*v.end()) << " - v.begin(): " << &(*v.begin()) << std::endl;
	// printSize(v);
}
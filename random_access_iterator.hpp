#include "iterator.hpp"

namespace ft
{
	template <typename T>
	class RandIter : public ft::iterator<ft::random_access_iterator_tag, T>
	{
	public:
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::iterator_category	iterator_category;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::value_type		value_type;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::pointer			pointer;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::reference			reference;
	private:
		pointer _ptr;
	public:
		// Default constructors
		RandIter(void) : _ptr(NULL) {}
		RandIter(pointer ptr) : _ptr(ptr) {}
		
		// Copy constructor
		RandIter(const RandIter& other) : _ptr(other._ptr) {}
		
		// Copy assignation
		RandIter& operator=(const RandIter& rhs) { if (this != &rhs) this->_ptr = other._ptr; return *this; }

		// Destructor
		~RandIter() {}

		// Dereference operators
		reference operator*() { return *this->_ptr; }
		
	};
};
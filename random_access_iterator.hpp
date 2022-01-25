#include "iterator_traits.hpp"

namespace ft
{
	template <typename T>
	class random_access_iterator : public ft::iterator<ft::random_access_iterator_tag, T>
	{
	public:
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::iterator_category	iterator_category;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::value_type		value_type;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::difference_type	difference_type;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::pointer			pointer;
		typedef typename ft::iterator<ft::random_access_iterator_tag, T>::reference			reference;
	protected:
		pointer _ptr;
	public:
		// Default constructors
		random_access_iterator(void) : _ptr(NULL) {}
		random_access_iterator(pointer ptr) : _ptr(ptr) {}
		
		// Copy constructor
		random_access_iterator(const random_access_iterator& other) : _ptr(other._ptr) {}
		
		// Copy assignation
		random_access_iterator& operator=(const random_access_iterator& rhs) { if (this != &rhs) this->_ptr = other._ptr; return *this; }

		// Destructor
		~random_access_iterator() {}

		// Member functions
		pointer const& base() const { return this->_ptr; }

		// Dereference operators
		reference operator*() const { return *this->_ptr; }
		pointer operator->() const { return &(operator*()); }
		reference operator[](difference_type n) const { return this->_ptr[n]; }

		// Addition operator
		random_access_iterator operator+(difference_type n) const { return random_access_iterator(this->_ptr + n); }

		// Increment operators
		random_access_iterator& operator++() { ++this->_ptr; return *this; }
		random_access_iterator operator++(int) { random_access_iterator tmp(*this); ++(*this); return tmp; }

		// Advance operator
		random_access_iterator& operator+=(difference_type n) { this->_ptr += n; return *this; }

		// Substraction operator
		random_access_iterator operator-(difference_type n) const { return random_access_iterator(this->_ptr - n); }

		// Decrement operators
		random_access_iterator& operator--() { --this->_ptr; return *this; }
		random_access_iterator operator--(int) { random_access_iterator tmp(*this); --(*this); return tmp; }

		// Retrocede operator
		random_access_iterator& operator-=(difference_type n) { this->_ptr -= n; return *this;}
	};

	// Relational operators
	template <class Iterator1, class Iterator2>
	bool operator==(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs) { return lhs.base() == rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator!=(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs) { return lhs.base() != rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator<(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs) { return lhs.base() < rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator<=(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs) { return lhs.base() <= rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator>(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs) { return lhs.base() > rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator>=(const random_access_iterator<Iterator1>& lhs, const random_access_iterator<Iterator2>& rhs) { return lhs.base() >= rhs.base(); }

	// Addition operator overload
	template <class Iterator>
	random_access_iterator<Iterator> operator+(typename random_access_iterator<Iterator>::difference_type n, const random_access_iterator<Iterator>& it)
	{
		return it + n;
	}

	// Substraction operator overload
	template <class Iterator>
	typename random_access_iterator<Iterator>::difference_type operator-(const random_access_iterator<Iterator>& lhs, const random_access_iterator<Iterator>& rhs)
	{
		return lhs.base() - rhs.base();
	}
};
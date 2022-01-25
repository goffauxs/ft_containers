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
	protected:
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

		// Member functions
		pointer const& base() const { return this->_ptr; }

		// Dereference operators
		reference operator*() const { return *this->_ptr; }
		pointer operator->() const { return &(operator*()); }
		reference operator[](difference_type n) const { return this->_ptr[n]; }

		// Addition operator
		RandIter operator+(difference_type n) const { return RandIter(this->_ptr + n); }

		// Increment operators
		RandIter& operator++() { ++this->_ptr; return *this; }
		RandIter operator++(int) { RandIter tmp(*this); ++(*this); return tmp; }

		// Advance operator
		RandIter& operator+=(difference_type n) { this->_ptr += n; return *this; }

		// Substraction operator
		RandIter operator-(difference_type n) const { return RandIter(this->_ptr - n); }

		// Decrement operators
		RandIter& operator--() { --this->_ptr; return *this; }
		RandIter operator--(int) { RandIter tmp(*this); --(*this); return tmp; }

		// Retrocede operator
		RandIter& operator-=(difference_type n) { this->_ptr -= n; return *this;}
	};

	// Relational operators
	template <class Iterator1, class Iterator2>
	bool operator==(const RandIter<Iterator1>& lhs, const RandIter<Iterator2>& rhs) { return lhs.base() == rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator!=(const RandIter<Iterator1>& lhs, const RandIter<Iterator2>& rhs) { return lhs.base() != rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator<(const RandIter<Iterator1>& lhs, const RandIter<Iterator2>& rhs) { return lhs.base() < rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator<=(const RandIter<Iterator1>& lhs, const RandIter<Iterator2>& rhs) { return lhs.base() <= rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator>(const RandIter<Iterator1>& lhs, const RandIter<Iterator2>& rhs) { return lhs.base() > rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator>=(const RandIter<Iterator1>& lhs, const RandIter<Iterator2>& rhs) { return lhs.base() >= rhs.base(); }

	// Addition operator overload
	template <class Iterator>
	RandIter<Iterator> operator+(typename RandIter<Iterator>::difference_type n, const RandIter<Iterator>& it)
	{
		return it + n;
	}

	// Substraction operator overload
	template <class Iterator>
	typename RandIter<Iterator>::difference_type operator-(const RandIter<Iterator>& lhs, const RandIter<Iterator>& rhs)
	{
		return lhs.base() - rhs.base();
	}
};
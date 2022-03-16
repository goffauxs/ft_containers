#ifndef REVERSE_ITERATOR_H
# define REVERSE_ITERATOR_H
# include "iterator_traits.hpp"

namespace ft
{
	template <class Iterator>
	class reverse_iterator
	{
	public:
		typedef 			Iterator										iterator_type;
		typedef typename	iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename	iterator_traits<Iterator>::value_type			value_type;
		typedef typename	iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename	iterator_traits<Iterator>::pointer				pointer;
		typedef typename	iterator_traits<Iterator>::reference			reference;
	protected:
		iterator_type	original;
	public:
		// Default constructor
		reverse_iterator() : original() {}

		// Copy constructor from original iterator
		explicit reverse_iterator(iterator_type it) : original(it) {}

		// Copy constructor from reverse iterator
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& rev_it) : original(rev_it.base()) {}

		// Casting operator
		operator reverse_iterator<const Iterator>() const { return this->original; }
	
		// Destructor
		virtual ~reverse_iterator() {}

		// Member functions
		iterator_type base() const { return this->original; }

		// Dereference operators
		reference operator*() const { iterator_type tmp(this->original); return *(--tmp); }
		pointer operator->() const { return &(operator*()); }
		reference operator[](difference_type n) const { return *(*this + n); }

		// Addition operator
		reverse_iterator operator+(difference_type n) const { return reverse_iterator(this->original - n); }

		// Increment operators
		reverse_iterator& operator++() { --this->original; return *this; }
		reverse_iterator operator++(int) { reverse_iterator tmp(*this); --this->original; return tmp; }

		// Advance operator
		reverse_iterator& operator+=(difference_type n) { this->original -= n; return *this; }

		// Substruction operator
		reverse_iterator operator-(difference_type n) const { return reverse_iterator(this->original + n); }

		// Decrement operators
		reverse_iterator& operator--() { ++this->original; return *this; }
		reverse_iterator operator--(int) { reverse_iterator tmp(*this); ++this->original; return tmp; }

		// Retrocede operator
		reverse_iterator& operator-=(difference_type n) { this->original += n; return *this; }
	};

	// Relational operators
	template <class Iterator1, class Iterator2>
	bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() == rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() != rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() > rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() >= rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() < rhs.base(); }

	template <class Iterator1, class Iterator2>
	bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) { return lhs.base() <= rhs.base(); }

	// Addition operator overload
	template <class Iterator>
	reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& rev_it)
	{
		return rev_it + n;
	}

	// Subtraction operator overload
	template <class Iterator1, class Iterator2>
	typename reverse_iterator<Iterator1>::difference_type operator-(const reverse_iterator<Iterator2>& lhs, const reverse_iterator<Iterator1>& rhs)
	{
		return rhs.base() - lhs.base();
	}
}


#endif /* REVERSE_ITERATOR_H */

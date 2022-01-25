#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include <memory>

namespace ft
{
	template <class T, class Alloc = std::allocator<T> >
	class vector
	{
	public:
		typedef				T												value_type;
		typedef				Alloc											allocator_type;
		typedef typename	allocator_type::reference						reference;
		typedef typename	allocator_type::const_reference					const_reference;
		typedef typename	allocator_type::pointer							pointer;
		typedef typename	allocator_type::const_pointer					const_pointer;
		typedef typename	ft::random_access_iterator<value_type>			iterator;
		typedef typename	ft::random_access_iterator<const value_type>	const_iterator;
		typedef typename	ft::reverse_iterator<iterator>					reverse_iterator;
		typedef typename	ft::reverse_iterator<const_iterator>			const_reverse_iterator;
		typedef typename	ft::iterator_traits<iterator>::difference_type	difference_type;
		typedef 			std::size_t										size_type;
	private:
		allocator_type	_alloc;
		pointer			_array;
		size_type		_capacity;
		size_type		_size;
	public:
		// Default constructor
		explicit vector(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _array(NULL), _capacity(0), _size(0) {}
		
		// Fill constructor
		explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc)
		{
			if (n < 0) throw std::out_of_range("vector");
			this->_size = n;
			this->_capacity = n;
			this->_array = this->_alloc.allocate(n);
			// TODO finish this function
		}
	};
}
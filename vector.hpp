#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "utils.hpp"
#include <memory>

namespace ft
{
	template <class T, class Alloc = std::allocator<T>>
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
		pointer			_start;
		pointer			_finish;
		pointer			_end_of_storage;
	public:
		// Default constructor
		explicit vector(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _start(NULL), _finish(NULL), _end_of_storage(NULL) {}
		
		// Fill constructor
		explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc)
		{
			if (n < 0) throw std::out_of_range("vector");
			this->_start = this->_alloc.allocate(n);
			this->_finish = this->_start;
			this->_end_of_storage = this->_start + n;
			for (; n > 0; --n) this->_alloc.construct(this->_finish++, val);
		}

		// Range constructor
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			size_type n = ft::distance(first, last);
			this->_start = this->_alloc.allocate(n);
			this->_finish = this->_start;
			this->_end_of_storage = this->_start + n;
			for (; first != last; first++, this->_finish++) this->_alloc.construct(this->_finish, *first);
		}

		// Copy constructor
		vector(const vector& other)
			: _alloc(other._alloc)
		{
			size_type n = other.size();
			this->_start = this->_alloc.allocate(n);
			this->_finish = this->_start;
			this->_end_of_storage = this->_start + n;
			for (iterator it = other.begin(); it != other.end(); it++, this->_finish++) this->_alloc.construct(this->_finish, *it);
		}

		// Assignement operator
		vector& operator=(const vector& rhs)
		{

		}

		/*
		/ ITERATORS
		*/

		// begin
		iterator		begin() 		{ return this->_start; }
		const_iterator	begin() const	{ return this->_start; }

		// end
		iterator		end() 		{ return this->_finish; }
		const_iterator	end() const { return this->_finish; }

		// rbegin
		reverse_iterator		rbegin()		{ return reverse_iterator(this->begin()); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(this->begin()); }

		// rend
		reverse_iterator		rend()			{ return reverse_iterator(this->_end()); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(this->end()); }

		/*
		/ CAPACITY
		*/

		// size
		size_type size() const { return size_type(this->_finish - this->_start); }

		// max_size
		size_type max_size() const { return this->_alloc.max_size(); }

		// resize
		void resize(size_type n, value_type val = value_type())
		{
			if (n > this->size())
				
			else if (n < this->size())

		}

		// capacity
		size_type capacity() const { return size_type(this->_end_of_storage - this->_start); }

		// empty
		bool empty() const { return this->size() == 0; }

		// reserve
		void reserve(size_type n)
		{
			if (n > this->max_size())
				throw std::length_error("vector::reserve");
			if (n > this->capacity())
			{
				iterator old_start = this->begin();
				iterator old_end = this->end();
				size_type old_capacity = this->capacity();

				this->_start = this->_alloc.allocate(n);
				this->_finish = this->_start;
				this->_end_of_storage = this->_start + n;
				for (iterator it = old_start; it != old_end; it++, this->_finish++) this->_alloc.construct(this->_finish, *it);
				for (; old_end != old_start; old_end--) this->_alloc.destroy(&(*old_end));
				this->_alloc.deallocate(&(*old_start), old_capacity);
			}
		}

		/*
		/ MODIFIERS
		*/

		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
			ft::enable_if<!ft::is_integral<InputIterator>::value>::true* = NULL)
		{
			
		}

		// insert
		iterator insert(iterator position, const value_type& val)
		{
			
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			if (n == 0)
				return ;
			if (this->size() + n > this->capacity())
				this->reserve(this->size() + n);
			iterator end_pos = position + n;
			for (iterator it = position; it != end_pos; it++, end_pos++)
			{
				this->_alloc.construct(&(*end_pos), *it);
				this->_alloc.destroy(&(*it));
			}
			for (size_type i = 0; i < n; i++)
				this->_alloc.construct((&(*position) + i), val);
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
			ft::enable_if<ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			
		}
	};
}
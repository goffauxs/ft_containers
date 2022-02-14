#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "utils.hpp"
#include <memory>

namespace ft
{
	template < class T, class Alloc = std::allocator<T> >
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
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
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
			if (this != &rhs)
				this->assign(rhs.begin(), rhs.end());
			return *this;
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
			if (n > this->max_size())
				throw std::out_of_range("vector::resize");
			if (n > this->size())
				this->insert(this->end(), n - this->size(), val);
			else if (n < this->size())
			{
				for (iterator it = this->end(); it != this->begin() + n; it--, this->_finish--)
					this->_alloc.destroy(this->_finish);
			}
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
		/ ELEMENT ACCESS
		*/

		// operator[]
		reference operator[](size_type n) { return this->_start[n]; }
		const_reference operator[](size_type n) const { return this->_start[n]; }

		// at
		reference at(size_type n)
		{
			if (n > this->size())
				throw std::out_of_range("vector::at")
			return this->_start[n];
		}

		const_reference at(size_type n) const
		{
			if (n > this->size())
				throw std::out_of_range("vector::at")
			return this->_start[n];
		}

		// front
		reference front() { return *this->_start; }
		const_reference front() const { return *this->_start; }

		// back
		reference back() { return *(this->_finish - 1); }
		const_reference back() const { return *(this->_finish - 1); }

		/*
		/ MODIFIERS
		*/

		// assign
		template <class InputIterator>
		void assign(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			difference_type n = ft::distance(first, last);
			if (n > this->capacity())
			{
				this->reserve(n);
				this->_finish = this->_start;
			}
			else
				this->clear();
			for (iterator it = first; it != last; it++, this->_finish++)
				this->_alloc.construct(this->finish, *it);
		}

		void assign(size_type n, const value_type& val)
		{
			if (n > this->capacity())
			{
				this->reserve(n);
				this->_finish = this->_start;
			}
			else
				this->clear();
			while (n--)
				this->_alloc.construct(this->_finish++, val);
		}

		// push_back
		void push_back(const value_type& val)
		{
			if (this->_end_of_storage == this->_finish)
			{
				if (!this->capacity())
					this->reserve(1);
				else
					this->reserve(this->capacity() * 2);
			}
			this->_alloc.construct(this->_finish++, val);
		}

		// pop_back
		void pop_back() { this->_alloc.destroy(--this->_finish); }

		// insert
		iterator insert(iterator position, const value_type& val)
		{
			this->insert(position, 1, val);
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
			for (difference_type i = 0; i < n; i++)
				this->_alloc.construct((&(*position) + i), val);
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if<ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			difference_type n = ft::distance(first, last);
			if (n <= 0)
				return ;
			if (this->size() + n > this->capacity())
				this->reserve(this->size() + n);
			iterator end_pos = position + n;
			for (iterator it = position; it != end_pos; it++, end_pos++)
			{
				this->_alloc.construct(&(*end_pos), *it);
				this->_alloc.destroy(&(*it));
			}
			for (difference_type i = 0; first != last; first++, i++)
				this->_alloc.construct((&(*position) + i), *first);
		}

		// erase
		iterator erase(iterator position)
		{
			iterator tmp = position;
			this->_alloc.destroy(position);
			for (; tmp != this->end(); tmp++)
				*tmp = *(tmp + 1);
			this->pop_back();
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			for (iterator it = first; it != last; it++)
				this->_alloc.destroy(it);
			iterator tmp = first;
			for (iterator it = tmp + 1; it != last; it++, tmp++)
				
		}

		// clear
		void clear()
		{
			while (this->_start != this->_finish)
			{
				this->_alloc.destroy(this->_finish);
				this->_finish--;
			}
		}
	};
}
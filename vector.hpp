#include "random_access_iterator.hpp"
#include "reverse_iterator.hpp"
#include "type_traits.hpp"
#include "utils.hpp"
#include <memory>
#include <iostream>

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

		size_type check_len(size_type n, const char* s) const
		{
			if (this->max_size() - this->size() < n)
				throw std::length_error(s);
			const size_type len = this->size() + std::max(this->size(), n);
			return (len < this->size() || len > this->max_size()) ? this->max_size() : len;
		}
		void destroy(iterator first, iterator last)
		{
			for (; first != last; ++first)
				this->_alloc.destroy(&(*first));
		}
		void move_range(pointer from_s, pointer from_e, pointer to)
		{
			pointer old_last = this->_finish;
			difference_type n = old_last - to;
			for (pointer i = from_s + n; i < from_e; ++i, ++this->_finish)
				this->_alloc.construct(&(*this->_finish), *i);
			this->move_backward(from_s, from_s + n, old_last);
		}
		iterator move_backward(iterator first, iterator last, iterator result)
		{
			while (last != first) *(--result) = *(--last);
			return result;
		}
		void construct_at_end(size_type n, const value_type& x)
		{
			while (n > 0)
			{
				this->_alloc.construct(&(*this->_finish), x);
				++this->_finish;
				--n;
			}
		}
		template <typename InputIterator>
		void construct_at_end(InputIterator first, InputIterator last)
		{
			for (; first != last; ++first, ++this->_finish)
				this->_alloc.construct(&(*this->_finish), *first);
		}
		void construct_forward(pointer begin1, pointer end1, pointer& begin2)
		{
			for (; begin1 != end1; ++begin1, ++begin2)
				this->_alloc.construct(&(*begin2), *begin1);
		}
		void construct_backward(pointer begin1, pointer end1, pointer& end2)
		{
			while (end1 != begin1)
			{
				this->_alloc.construct(&(*(end2 - 1)), *--end1);
				--end2;
			}
		}
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
			: _alloc(alloc)
		{
			difference_type n = ft::distance(first, last);
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
			pointer p = other._start;
			while (n--) this->_alloc.construct(this->_finish++, *p++);
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
				for (; old_end != old_start; old_end--) this->_alloc.destroy(old_end.base());
				this->_alloc.deallocate(old_start.base(), old_capacity);
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
				throw std::out_of_range("vector::at");
			return this->_start[n];
		}

		const_reference at(size_type n) const
		{
			if (n > this->size())
				throw std::out_of_range("vector::at");
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
			size_type n = ft::distance(first, last);
			if (n > this->capacity())
			{
				this->reserve(n);
				this->_finish = this->_start;
			}
			else
				this->clear();
			while (n--) this->_alloc.construct(this->_finish++, *first++);
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
			return position;
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			pointer p = &(*position);
			if (n > 0)
			{
				if (size_type(this->_end_of_storage - this->_finish) >= n)
				{
					const size_type elems_after = this->end() - position;
					size_type old_n = n;
					pointer old_finish(this->_finish);
					if (n > elems_after)
					{
						size_type diff = n - elems_after;
						this->construct_at_end(diff, val);
						n -= diff;
					}
					if (n > 0)
					{
						difference_type diff = old_finish - (p + n);
						for (pointer i = p + diff; i < old_finish; ++i, ++this->_finish)
							this->_alloc.construct(&(*this->_finish), *i);
						this->move_backward(p, p + diff, old_finish);
						const_pointer xr = static_cast<const_pointer>(val);
						if (p <= xr && xr < this->_finish)
							xr += old_n;
						std::fill_n(p, n, *xr);
					}
				}
				else
				{
					size_type len = this->check_len(n, "vector::insert");
					const size_type elems_before = position - this->begin();
					pointer first(this->_alloc.allocate(len));
					pointer new_end_cap(first + len);
					pointer new_start(first + elems_before);
					pointer new_finish(new_start);
					while (n > 0)
					{
						this->_alloc.construct(&(*new_finish), val);
						++new_finish;
						--n;
					}
					this->construct_backward(this->_start, p, new_start);
					this->construct_forward(p, this->_finish, new_finish);
					std::swap(this->_start, new_start);
					std::swap(this->_finish, new_finish);
					std::swap(this->_end_of_storage, new_end_cap);
					// TODO probably delete the contents of the old vector
				}
			}
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			pointer p = &(*position);
			difference_type n = ft::distance(first, last);
			if (n > 0)
			{
				if (n <= this->_end_of_storage - this->_finish)
				{
					size_type old_n = n;
					pointer old_last = this->_finish;
					InputIterator m = last;
					difference_type elems_after = this->_finish - p;
					if (n > elems_after)
					{
						m = first;
						difference_type diff = this->_finish - p;
						ft::advance(m, diff);
						this->construct_at_end(first, last);
						n = elems_after;
					}
					if (n > 0)
					{
						move_range(p, old_last, p + old_n);
						std::copy(first, m, p);
					}
				}
				else
				{
					size_type len = this->check_len(n, "vector::insert");
					const size_type elems_before = position - this->begin();
					pointer new_first(this->_alloc.allocate(len));
					pointer new_end_cap(new_first + len);
					pointer new_start(new_first + elems_before);
					pointer new_finish(new_start);
					for (; first != last; ++first, ++new_finish)
						this->_alloc.construct(new_finish, *first);
					this->construct_backward(this->_start, p, new_start);
					this->construct_forward(p, this->_finish, new_finish);
					std::swap(this->_start, new_start);
					std::swap(this->_finish, new_finish);
					std::swap(this->_end_of_storage, new_end_cap);
				}
			}
		}

		// erase
		iterator erase(iterator position)
		{
			iterator tmp = position;
			this->_alloc.destroy(tmp.base());
			for (iterator it = tmp + 1; it != this->end(); it++, tmp++)
				*tmp = *it;
			this->_alloc.destroy(--this->_finish);
			return position;
		}

		iterator erase(iterator first, iterator last)
		{
			iterator tmp = first;
			for (; tmp != last; tmp++) this->_alloc.destroy(&(*tmp));
			tmp = first;
			difference_type n = 0;
			for (; tmp != last && last != this->end(); tmp++, last++, n++)
			{
				*tmp = *last;
				this->_alloc.destroy(&(*last));
			}
			this->_finish -= n;
			return first;
		}

		// swap
		void swap(vector& other)
		{
			pointer tmp = this->_start;
			this->_start = other._start;
			other._start = tmp;
			tmp = this->_finish;
			this->_finish = other._finish;
			other._finish = tmp;
			tmp = this->_end_of_storage;
			this->_end_of_storage = other._end_of_storage;
			other._end_of_storage = tmp;
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
#ifndef VECTOR_H
# define VECTOR_H
# include "../iterator/random_access_iterator.hpp"
# include "../iterator/reverse_iterator.hpp"
# include "../utils/type_traits.hpp"
# include "../utils/utils.hpp"
# include <memory>
# include <iostream>

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

		template <typename InputIterator>
		pointer allocate_and_copy(size_type n, InputIterator first, InputIterator last)
		{
			pointer result = this->_alloc.allocate(n);
			std::uninitialized_copy(first, last, result);
			return result;
		}
		size_type check_len(size_type n, const char* s) const
		{
			if (this->max_size() - this->size() < n)
				throw std::length_error(s);
			const size_type len = this->size() + std::max(this->size(), n);
			return (len < this->size() || len > this->max_size()) ? this->max_size() : len;
		}
		template <typename InputIterator>
		void destroy(InputIterator first, InputIterator last)
		{
			for (; first != last; ++first)
				this->_alloc.destroy(&(*first));
		}
		template <class InputIterator, class OutputIterator>
		OutputIterator move(InputIterator first, InputIterator last, OutputIterator result)
		{
			while (first != last)
			{
				*result = *first;
				++result;
				++first;
			}
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
		void destruct_at_end(pointer new_last)
		{
			pointer soon_to_be_end = this->_finish;
			while (new_last != soon_to_be_end)
				this->_alloc.destroy(--soon_to_be_end);
			this->_finish = new_last;
		}
		void construct_forward(pointer begin1, pointer end1, pointer& begin2)
		{
			for (; begin1 != end1; ++begin1, ++begin2)
				this->_alloc.construct(&(*begin2), *begin1);
		}
		void construct_backward(pointer begin1, pointer end1, pointer& end2)
		{
			while (end1 != begin1)
				this->_alloc.construct(&(*(--end2)), *(--end1));
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

		~vector()
		{
			this->destroy(this->_start, this->_finish);
			this->_alloc.deallocate(this->_start, this->_end_of_storage - this->_start);
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
		reverse_iterator		rbegin()		{ return reverse_iterator(this->end()); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(this->end()); }

		// rend
		reverse_iterator		rend()			{ return reverse_iterator(this->begin()); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(this->begin()); }

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
				const size_type old_size = size();
				pointer tmp = allocate_and_copy(n, this->_start, this->_finish);
				this->destroy(this->_start, this->_finish);
				this->_alloc.deallocate(this->_start, this->_end_of_storage - this->_start);
				this->_start = tmp;
				this->_finish = tmp + old_size;
				this->_end_of_storage = this->_start + n;
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
			const size_type n = position - begin();
			insert(position, 1, val);
			return iterator(this->_start + n);
		}

		void insert(iterator position, size_type n, const value_type& val)
		{
			if (n > 0)
			{
				if (size_type(this->_end_of_storage - this->_finish) >= n) // no need to allocate
				{
					const size_type elems_after = end() - position;
					pointer old_finish(this->_finish);
					if (elems_after > n)
					{
						std::uninitialized_copy(this->_finish - n, this->_finish, this->_finish);
						this->destroy(this->_finish - n, this->_finish);
						this->_finish += n;
						this->construct_backward(position.base(), old_finish - n, old_finish);
						this->destroy(position.base(), old_finish - n);
						std::fill(position.base(), position.base() + n, val);
					}
					else
					{
						this->_finish = std::uninitialized_fill_n(this->_finish, n - elems_after, val);
						std::uninitialized_copy(position.base(), old_finish, this->_finish);
						this->destroy(position.base(), old_finish);
						this->_finish += elems_after;
						this->destroy(position.base(), old_finish);
					}
				}
				else // need to allocate
				{
					const size_type len = check_len(n, "vector::insert");
					const size_type elems_before = position - begin();
					pointer new_start(this->_alloc.allocate(len));
					pointer new_finish(new_start);
					std::uninitialized_fill_n(new_start + elems_before, n, val);
					new_finish = std::uninitialized_copy(this->_start, position.base(), new_start);
					this->destroy(this->_start, position.base());
					new_finish += n;
					new_finish = std::uninitialized_copy(position.base(), this->_finish, new_finish);
					this->destroy(this->_start, this->_finish);
					this->_alloc.deallocate(this->_start, this->_end_of_storage - this->_start);
					this->_start = new_start;
					this->_finish = new_finish;
					this->_end_of_storage = new_start + len;
				}
			}
		}

		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			if (first != last)
			{
				const size_type n = ft::distance(first, last);
				if (n <= size_type(this->_end_of_storage - this->_finish))
				{
					const size_type elems_after = end() - position;
					pointer old_finish(this->_finish);
					if (elems_after > n)
					{
						std::uninitialized_copy(this->_finish - n, this->_finish, this->_finish);
						this->destroy(this->_finish - n, this->_finish);
						this->_finish += n;
						std::copy_backward(position.base(), old_finish - n, old_finish);
						this->destroy(position.base(), old_finish - n);
						std::copy(first, last, position);
					}
					else
					{
						InputIterator mid = first;
						ft::advance(mid, elems_after);
						std::uninitialized_copy(mid, last, this->_finish);
						this->_finish += n - elems_after;
						std::uninitialized_copy(position.base(), old_finish, this->_finish);
						this->destroy(position.base(), old_finish);
						this->_finish += elems_after;
						std::copy(first, mid, position);
					}
				}
				else
				{
					const size_type len = this->check_len(n, "vector::insert");
					pointer new_start(this->_alloc.allocate(len));
					pointer new_finish(new_start);
					new_finish = std::uninitialized_copy(this->_start, position.base(), new_start);
					this->destroy(this->_start, position.base());
					new_finish = std::uninitialized_copy(first, last, new_finish);
					new_finish = std::uninitialized_copy(position.base(), this->_finish, new_finish);
					this->destroy(position.base(), this->_finish);
					this->destroy(this->_start, this->_finish);
					this->_alloc.deallocate(this->_start, this->_end_of_storage - this->_start);
					this->_start = new_start;
					this->_finish = new_finish;
					this->_end_of_storage = new_start + len;
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
			pointer p = this->_start + (first - this->begin());
			if (first != last)
				this->destruct_at_end(this->move(p + (last - first), this->_finish, p));
			return iterator(p);
		}

		// swap
		void swap(vector& other)
		{
			std::swap(_start, other._start);
			std::swap(_finish, other._finish);
			std::swap(_end_of_storage, other._end_of_storage);
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

		/*
		/ ALLOCATOR
		*/

		// get_allocator
		allocator_type get_allocator() const { return this->_alloc; }
	};

	template <class T, class Alloc>
	bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return (lhs.size() == rhs.size() 
				&& ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class T, class Alloc>
	bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs)
	{
		return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>
	bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs == rhs); }

	template <class T, class Alloc>
	bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return rhs < lhs; }

	template <class T, class Alloc>
	bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(rhs < lhs); }

	template <class T, class Alloc>
	bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs < rhs); }

	template <class T, class Alloc>
	void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) { x.swap(y); }
}

#endif /* VECTOR_H */

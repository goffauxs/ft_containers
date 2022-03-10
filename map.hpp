#pragma once
#include <functional>
#include <memory>
#include "pair.hpp"
#include "RBTree.hpp"

namespace ft
{
	template <typename Key, typename T, typename Compare = std::less<Key>, typename Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	public:
		typedef Key						key_type;
		typedef T						mapped_type;
		typedef ft::pair<const Key, T>	value_type;
		typedef Compare					key_compare;
		typedef Alloc					allocator_type;
	public:
		class value_compare : public ft::binary_function<value_type, value_type, bool>
		{
			friend class map<Key, T, Compare, Alloc>;
		protected:
			Compare _comp;

			value_compare(Compare c) : _comp(c) {}
		
		public:
			bool operator()(const value_type& x, const value_type& y) const { return _comp(x.first, y.first); }
		};
	private:
		typedef typename Alloc::rebind<value_type>::other Pair_alloc_type;

		typedef RBTree<key_type, value_type, key_compare, Pair_alloc_type>	Rep_type;

		Rep_type _t;
	public:
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename Rep_type::iterator					iterator;
		typedef typename Rep_type::const_iterator			const_iterator;
		typedef typename Rep_type::size_type				size_type;
		typedef typename Rep_type::difference_type			difference_type;
		typedef typename Rep_type::reverse_iterator			reverse_iterator;
		typedef typename Rep_type::const_reverse_iterator	const_reverse_iterator;

		// Default Constructor
		map() : _t() {}

		// Parametrized Constructor
		explicit map(const Compare& comp, const allocator_type& a = allocator_type()) : _t(comp, Pair_alloc_type(a)) {}

		// Copy Constructor
		map(const map& other) : _t(other._t) {}

		// Range Constructors
		template <typename InputIterator>
		map(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			: _t()
		{
			_t.insert_unique(first, last);
		}

		template <typename InputIterator>
		map(InputIterator first, InputIterator last, const Compare& comp, const allocator_type& a = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			: _t(comp, Pair_alloc_type(a))
		{
			_t.insert_unique(first, last);
		}

		// Assignment operator overload
		map& operator=(const map& rhs)
		{
			if (this != &rhs)
				this->_t = rhs._t;
			return *this;
		}

		// get_allocator
		allocator_type get_allocator() const { return allocator_type(this->_t.get_allocator()); }

		// iterators
		iterator		begin()			{ return this->_t.begin(); }
		const_iterator	begin() const	{ return this->_t.begin(); }

		iterator		end()			{ return this->_t.end(); }
		const_iterator	end() const		{ return this->_t.end(); }

		reverse_iterator		rbegin()			{ return this->_t.rbegin(); }
		const_reverse_iterator	rbegin()	const	{ return this->_t.rbegin(); }

		reverse_iterator		rend()			{ return this->_t.rend(); }
		const_reverse_iterator	rend()	const	{ return this->_t.rend(); }

		// capacity
		bool empty() const { return this->_t.empty(); }
		size_type size() const { return this->_t.size(); }
		size_type max_size() const { return this->_t.max_size(); }

		// element access
		mapped_type& operator[](const key_type& k)
		{
			iterator i = this->lower_bound(k);
			if (i == this->end() || key_comp()(k, (*i).first))
				i = this->insert(i, value_type(k, mapped_type()));
			return (*i).second;
		}

		// modifiers
		ft::pair<iterator, bool> insert(const value_type& x) { return this->_t.insert_unique(x); }

		iterator insert(iterator position, const value_type& x) { return this->_t.insert_unique(position, x); }

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			this->_t.insert_unique(first, last);
		}

		void erase(iterator position) { this->_t.erase(position); }

		size_type erase(const key_type& k) { return this->_t.erase(k); }

		void erase(iterator first, iterator last) { this->_t.erase(first, last); }

		void swap(map& x) { this->_t.swap(x._t); }

		void clear() { this->_t.clear(); }

		key_compare key_comp() const { return this->_t.key_comp(); }

		value_compare value_comp() const { return value_compare(this->_t.key_comp()); }

		iterator find(const key_type& k) { return this->_t.find(k); }

		const_iterator find(const key_type& k) const { return this->_t.find(k); }

		size_type count(const key_type& x) const { return this->_t.find(x) == this->_t.end() ? 0 : 1; }

		iterator lower_bound(const key_type& k) { return this->_t.lower_bound(k); }

		const_iterator lower_bound(const key_type& k) const { return this->_t.lower_bound(k); }

		iterator upper_bound(const key_type& k) { return this->_t.upper_bound(k); }

		const_iterator upper_bound(const key_type& k) const { return this->_t.upper_bound(k); }

		ft::pair<iterator, iterator> equal_range(const key_type& k) { return this->_t.equal_range(k); }

		ft::pair<const_iterator, const_iterator> equal_range(const key_type& k) const { return this->_t.equal_range(k); }

		template <typename K1, typename T1, typename C1, typename A1>
		friend bool operator==(const map<K1, T1, C1, A1>&, const map<K1, T1, C1, A1>&);

		template <typename K1, typename T1, typename C1, typename A1>
		friend bool operator<(const map<K1, T1, C1, A1>&, const map<K1, T1, C1, A1>&);
	};

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) { return lhs._t == rhs._t; }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator<(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) { return lhs._t < rhs._t; }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator!=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) { return !(lhs == rhs); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator>(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) { return rhs < lhs; }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator<=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) { return !(rhs < lhs); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	bool operator>=(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs) { return !(lhs < rhs); }

	template <typename Key, typename T, typename Compare, typename Alloc>
	void swap(map<Key, T, Compare, Alloc>& x, map<Key, T, Compare, Alloc>& y) { x.swap(y); }
}
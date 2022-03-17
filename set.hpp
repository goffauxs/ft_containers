#ifndef SET_H
# define SET_H
# include "RBTree.hpp"

namespace ft
{
	template <class Key, class Compare = ft::less<Key>, class Alloc = std::allocator<Key> >
	class set
	{
	public:
		typedef Key											key_type;
		typedef Key											value_type;
		typedef Compare										key_compare;
		typedef Compare										value_compare;
		typedef Alloc										allocator_type;
	private:
		typedef typename Alloc::template rebind<value_type>::other								key_alloc_type;
		typedef RBTree<key_type, value_type, Identity<value_type>, key_compare, key_alloc_type>	tree_type;	

		tree_type _t;
	public:
		typedef typename allocator_type::reference			reference;
		typedef typename allocator_type::const_reference	const_reference;
		typedef typename allocator_type::pointer			pointer;
		typedef typename allocator_type::const_pointer		const_pointer;
		typedef typename tree_type::const_iterator			iterator;
		typedef typename tree_type::const_iterator			const_iterator;
		typedef typename tree_type::size_type				size_type;
		typedef typename tree_type::difference_type			difference_type;
		typedef typename tree_type::const_reverse_iterator	reverse_iterator;
		typedef typename tree_type::const_reverse_iterator	const_reverse_iterator;

		// Parametrized Constructor
		explicit set(const key_compare& comp = key_compare(), const allocator_type& a = allocator_type()) : _t(comp, key_alloc_type(a)) {}

		// Copy Constructor
		set(const set& other) : _t(other._t) {}

		// Range Constructor
		template <class InputIterator>
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& a = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
			: _t(comp, key_alloc_type(a))
		{
			_t.insert_unique(first, last);
		}

		// Assignment operator overload
		set& operator=(const set& rhs)
		{
			if (this != &rhs)
				this->_t = rhs._t;
			return *this;
		}

		// allocator
		allocator_type get_allocator() const { return allocator_type(this->_t.get_allocator()); }

		//iterators
		iterator	begin()	const	{ return this->_t.begin(); }
		iterator	end() const		{ return this->_t.end(); }

		reverse_iterator	rbegin() const	{ return this->_t.rbegin(); }
		reverse_iterator	rend() const	{ return this->_t.rend(); }

		// capacity
		bool empty() const { return this->_t.empty(); }
		size_type size() const { return this->_t.size(); }
		size_type max_size() const { return this->_t.max_size(); }

		// modifiers
		ft::pair<iterator, bool> insert(const value_type& x) { return this->_t.insert_unique(x); }

		iterator insert(iterator position, const value_type& x) { return this->_t.insert_unique(position, x); }

		template <typename InputIterator>
		void insert(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = NULL)
		{
			this->_t.insert_unique(first, last);
		}

		void erase(iterator position) { this->_t.erase(position); }

		size_type erase(const value_type& v) { return this->_t.erase(v); }

		void erase(iterator first, iterator last) { this->_t.erase(first, last); }

		void swap(set& x) { this->_t.swap(x._t); }

		void clear() { this->_t.clear(); }

		// observers
		key_compare key_comp() const { return this->_t.key_comp(); }

		value_compare value_comp() const { return value_compare(this->_t.key_comp()); }

		// operations
		iterator find(const value_type& v) { return this->_t.find(v); }

		const_iterator find(const value_type& v) const { return this->_t.find(v); }

		size_type count(const value_type& v) const { return this->_t.find(v) == this->_t.end() ? 0 : 1; }

		iterator lower_bound(const value_type& v) { return this->_t.lower_bound(v); }

		const_iterator lower_bound(const value_type& v) const { return this->_t.lower_bound(v); }

		iterator upper_bound(const value_type& v) { return this->_t.upper_bound(v); }

		const_iterator upper_bound(const value_type& v) const { return this->_t.upper_bound(v); }

		ft::pair<iterator, iterator> equal_range(const value_type& v) { return this->_t.equal_range(v); }

		ft::pair<const_iterator, const_iterator> equal_range(const value_type& v) const { return this->_t.equal_range(v); }

		template <typename K, typename C, typename A>
		friend bool operator==(const set<K, C, A>& lhs, const set<K, C, A>& rhs);

		template <typename K, typename C, typename A>
		friend bool operator<(const set<K, C, A>& lhs, const set<K, C, A>& rhs);
	};

	template <typename Key, typename Compare, typename Alloc>
	bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) { return lhs._t == rhs._t; }

	template <typename Key, typename Compare, typename Alloc>
	bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) { return lhs._t < rhs._t; }

	template <typename Key, typename Compare, typename Alloc>
	bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) { return !(lhs == rhs); }

	template <typename Key, typename Compare, typename Alloc>
	bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) { return rhs < lhs; }

	template <typename Key, typename Compare, typename Alloc>
	bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) { return !(rhs < lhs); }

	template <typename Key, typename Compare, typename Alloc>
	bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) { return !(lhs < rhs); }

	template <typename Key, typename Compare, typename Alloc>
	void swap(set<Key, Compare, Alloc>& x, set<Key, Compare, Alloc>& y) { x.swap(y); }
}

#endif /* SET_H */

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
		typedef typename allocator_type::template rebind<value_type>::other Pair_alloc_type;

		typedef RBTree<key_type, value_type, 
	};
}
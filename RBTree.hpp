#pragma once
#include <memory>
#include "utils.hpp"
#include "pair.hpp"

namespace ft
{
	enum RB_tree_color { red = false, black = true };

	struct RB_tree_node_base
	{
		typedef RB_tree_node_base*	Base_ptr;
		typedef const RB_tree_node_base* Const_Base_ptr;

		RB_tree_color	color;
		Base_ptr		parent;
		Base_ptr		left;
		Base_ptr		right;

		static Base_ptr minimum(Base_ptr x)
		{
			while (x->left != 0) x = x->left;
			return x;
		}
		static Const_Base_ptr minimum(Const_Base_ptr x)
		{
			while (x->left != 0) x = x->left;
			return x;
		}
		static Base_ptr maximum(Base_ptr x)
		{
			while (x->right != 0) x = x->right;
			return x;
		}
		static Const_Base_ptr maximum(Const_Base_ptr x)
		{
			while (x->right != 0) x = x->right;
			return x;
		}
	};

	struct RB_tree_header
	{
		RB_tree_node_base	header;
		size_t				node_count;

		RB_tree_header()
		{
			header.color = red;
			reset();
		}

		void move_data(RB_tree_header& from)
		{
			header.color = from.header.color;
			header.parent = from.header.parent;
			header.left = from.header.left;
			header.right = from.header.right;
			header.parent->parent = &header;
			node_count = from.node_count;

			from.reset();
		}

		void reset()
		{
			header.parent = 0;
			header.left = &header;
			header.right = &header;
			node_count = 0;
		}
	};

	template <typename Val>
	struct RB_tree_node : public RB_tree_node_base
	{
		typedef RB_tree_node<Val>*	Link_type;

		Val	value_field;
		
				Val* valptr() 		{ return &value_field; }
		const	Val* valptr() const { return &value_field; }
	};

	RB_tree_node_base* RB_tree_increment(RB_tree_node_base* x) throw()
	{
		if (x->right != 0)
		{
			x = x->right;
			while (x->left != 0)
				x = x->left;
		}
		else
		{
			RB_tree_node_base* y = x->parent;
			while (x == y->right)
			{
				x = y;
				y = y->parent;
			}
			if (x->right != y)
				x = y;
		}
		return x;
	}

	RB_tree_node_base* RB_tree_decrement(RB_tree_node_base* x) throw()
	{
		if (x->color == red && x->parent->parent == x)
			x = x->right;
		else if (x->left != 0)
		{
			RB_tree_node_base* y = x->left;
			while (y->right != 0)
				y = y->right;
			x = y;
		}
		else
		{
			RB_tree_node_base* y = x->parent;
			while (x == y->left)
			{
				x = y;
				y = y->parent;
			}
			x = y;
		}
		return x;
	}

	template <typename T>
	struct RB_tree_iterator
	{
		typedef T	value_type;
		typedef T&	reference;
		typedef T*	pointer;

		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;

		typedef RB_tree_iterator<T>			self;
		typedef RB_tree_node_base::Base_ptr	Base_ptr;
		typedef RB_tree_node<T>*			Link_type;

		Base_ptr	node;

		RB_tree_iterator() : node() {}
		explicit RB_tree_iterator(Base_ptr x) : node(x) {}
		
		// Dereference operators
		reference operator*() const { return *static_cast<Link_type>(node)->valptr(); }
		pointer operator->() const { return static_cast<Link_type>(node)->valptr(); }
		
		// Increment/Decrement operators
		self& operator++()
		{
			node = RB_tree_increment(node);
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			node = RB_tree_increment(node);
			return tmp;
		}
		self& operator--()
		{
			node = RB_tree_decrement(node);
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			node = RB_tree_decrement(node);
			return tmp;
		}

		// Comparison operators
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
	};

	template <typename T>
	struct RB_tree_const_iterator
	{
		typedef T			value_type;
		typedef const T&	reference;
		typedef const T*	pointer;

		typedef RB_tree_iterator<T> iterator;

		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;

		typedef RB_tree_const_iterator<T>	self;
		typedef RB_tree_node_base::Base_ptr	Base_ptr;
		typedef const RB_tree_node<T>*		Link_type;

		Base_ptr node;

		RB_tree_const_iterator() : node() {}
		explicit RB_tree_const_iterator(Base_ptr x) : node(x) {}
		RB_tree_const_iterator(const iterator& it) : node(it.node) {}

		iterator const_cast() const { return iterator(const_cast<typename iterator::Base_ptr>(node)); }
		
		// Dereference operators
		reference operator*() const { return *static_cast<Link_type>(node)->valptr(); }
		pointer operator->() const { return static_cast<Link_type>(node)->valptr(); }

		// Increment/Decrement operators
		self& operator++()
		{
			node = RB_tree_increment(node);
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			node = RB_tree_increment(node);
			return tmp;
		}
		self& operator--()
		{
			node = RB_tree_decrement(node);
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			node = RB_tree_decrement(node);
			return tmp;
		}

		// Comparison operators
		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }
	};

	// Relational operators
	template <typename Val>
	bool operator==(const RB_tree_iterator<Val>& x, const RB_tree_const_iterator<Val>& y) { return x.node == y.node; }

	template <typename Val>
	bool operator!=(const RB_tree_iterator<Val>& x, const RB_tree_const_iterator<Val>& y) { return x.node != y.node; }

	void RB_tree_rotate_left(RB_tree_node_base* const x, RB_tree_node_base*& root)
	{
		RB_tree_node_base* const y = x->right;

		x->right = y->left;
		if (y->left != 0)
			y->left->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->right = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	void RB_tree_rotate_right(RB_tree_node_base* const x, RB_tree_node_base*& root)
	{
		RB_tree_node_base* const y = x->left;

		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->right)
			x->parent->right = y;
		else
			x->parent->left = y;
		y->right = x;
		x->parent = y;
	}

	void RB_tree_insert_and_rebalance(const bool insert_left, RB_tree_node_base* x, RB_tree_node_base* p, RB_tree_node_base& header) throw()
	{
		RB_tree_node_base *& root = header.parent;

		x->parent = p;
		x->left = 0;
		x->right = 0;
		x->color = red;

		if (insert_left)
		{
			p->left = x;
			if (p == &header)
			{
				header.parent = x;
				header.right = x;
			}
			else if (p == header.left)
				header.left = x;
		}
		else
		{
			p->right = x;
			if (p == header.right)
				header.right = x;
		}
		while (x != root && x->parent->color == red)
		{
			RB_tree_node_base* const xgp = x->parent->parent;
			if (x->parent == xgp->left)
			{
				RB_tree_node_base* const y = xgp->right;
				if (y && y->color == red)
				{
					x->parent->color = black;
					y->color = black;
					xgp->color = red;
					x = xgp;
				}
				else
				{
					if (x == x->parent->right)
					{
						x = x->parent;
						RB_tree_rotate_left(x, root);
					}
					x->parent->color = black;
					xgp->color = red;
					RB_tree_rotate_right(xgp, root);
				}
			}
			else
			{
				RB_tree_node_base* const y = xgp->left;
				if (y && y->color == red)
				{
					x->parent->color = black;
					y->color = black;
					xgp->color = red;
					x = xgp;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						RB_tree_rotate_right(x, root);
					}
					x->parent->color = black;
					xgp->color = red;
					RB_tree_rotate_left(xgp, root);
				}
			}
		}
		root->color = black;
	}

	RB_tree_node_base* RB_tree_rebalance_for_erase(RB_tree_node_base* const z, RB_tree_node_base& header) throw()
	{
		RB_tree_node_base *& root = header.parent;
		RB_tree_node_base *& leftmost = header.left;
		RB_tree_node_base *& rightmost = header.right;
		RB_tree_node_base* y = z;
		RB_tree_node_base* x = 0;
		RB_tree_node_base* x_parent = 0;

		if (y->left == 0)
			x = y->right;
		else
		{
			if (y->right == 0)
				x = y->left;
			else
			{
				y = y->right;
				while (y->left != 0)
					y = y->left;
				x = y->right;
			}
		}
		if (y != z)
		{
			z->left->parent = y;
			y->left = z->left;
			if (y != z->left)
			{
				x_parent = y->parent;
				if (x) x->parent = y->parent;
				y->parent->left = x;
				y->right = z->right;
				z->right->parent = y;
			}
			else
				x_parent = y;
			if (root == z)
				root = y;
			else if (z->parent->left == z)
				z->parent->left = y;
			else
				z->parent->right = y;
			y->parent = z->parent;
			std::swap(y->color, z->color);
			y = z;
		}
		else
		{
			x_parent = y->parent;
			if (x)
				x->parent = y->parent;
			if (root == z)
				root = x;
			else
			{
				if (z->parent->left == z)
					z->parent->left = x;
				else
					z->parent->left = x;
			}
			if (leftmost == z)
			{
				if (z->right == 0)
					leftmost = z->parent;
				else
					leftmost = RB_tree_node_base::minimum(x);
			}
			if (rightmost == z)
			{
				if (z->left == 0)
					rightmost = z->parent;
				else
					rightmost = RB_tree_node_base::maximum(x);
			}
		}
		if (y->color != red)
		{
			while (x != root && (x == 0 || x->color == black))
			{
				if (x == x_parent->left)
				{
					RB_tree_node_base* w = x_parent->right;
					if (w->color == red)
					{
						w->color = black;
						x_parent->color = red;
						RB_tree_rotate_left(x_parent, root);
						w = x_parent->right;
					}
					if ((w->left == 0 || w->left->color == black) && (w->right == 0 || w->right->color == black))
					{
						w->color = red;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else
					{
						if (w->right == 0 || w->right->color == black)
						{
							w->left->color = black;
							x->color = red;
							RB_tree_rotate_right(w, root);
							w = x_parent->right;
						}
						w->color = x_parent->color;
						x_parent->color = black;
						if (w->right)
							w->right->color = black;
						RB_tree_rotate_left(x_parent, root);
						break;
					}
				}
				else
				{
					RB_tree_node_base* w = x_parent->left;
					if (w->color == red)
					{
						w->color = black;
						x_parent->color = red;
						RB_tree_rotate_right(x_parent, root);
						w = x_parent->left;
					}
					if ((w->right == 0 || w->right->color == black) && (w->left == 0 || w->left->color == black))
					{
						w->color = red;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else
					{
						if (w->left == 0 || w->left->color == black)
						{
							w->right->color = black;
							w->color = red;
							RB_tree_rotate_left(w, root);
							w = x_parent->left;
						}
						w->color = x_parent->color;
						x_parent->color = black;
						if (w->left)
							w->left->color = black;
						RB_tree_rotate_right(x_parent, root);
						break;
					}
				}
			}
			if (x) x->color = black;
		}
		return y;
	}

	template <typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc = std::allocator<Val> >
	class RB_tree
	{
	protected:
		typedef RB_tree_node_base*			Base_ptr;
		typedef const RB_tree_node_base*	Const_Base_ptr;
		typedef RB_tree_node<Val>*			Link_type;
		typedef const RB_tree_node<Val>*	Const_Link_type;
	private:
		Base_ptr	_root;
		Base_ptr	_nodes;
		RB_tree&	_t;
	private:
		struct Reuse_or_alloc_node(RB_tree& t) : _root(t._root), _nodes(t.), 
	};
}
#pragma once
#include <memory>
#include "reverse_iterator.hpp"
#include "utils.hpp"
#include "pair.hpp"

namespace ft
{
	template <class T, class NodePtr>
	class tree_iter
	{
	public:
		typedef bidirectional_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef value_type&					reference;
		typedef NodePtr						pointer;
	private:
		explicit tree_iter(pointer p) : _ptr(p) {}

		pointer _ptr;
	public:
		tree_iter() : _ptr(nullptr) {}
		tree_iter(const tree_iter& other) : _ptr(other._ptr) {}
		
		reference operator*() const { return this->_ptr->content; }
		pointer operator->() const { return &this->operator*(); }

		tree_iter& operator++() { this->_ptr = tree_next_iter(this->_ptr); return *this; }
		tree_iter& operator--() { this->_ptr = tree_prev_iter(this->_ptr); return *this; }
		tree_iter& operator++(int) { tree_iter tmp(*this); ++(*this); return tmp; }
		tree_iter& operator--(int) { tree_iter tmp(*this); --(*this); return tmp; }

		friend bool operator==(const tree_iter& lhs, const tree_iter& rhs) { return lhs._ptr == rhs._ptr; }
		friend bool operator!=(const tree_iter& lhs, const tree_iter& rhs) { return !(lhs == rhs); }
	};

	template <class NodePtr>
	bool tree_is_left_child(NodePtr x) { return x == x->parent->left; }

	template <class NodePtr>
	NodePtr tree_min(NodePtr x)
	{
		while (x->left != nullptr)
			x = x->left;
		return x;
	}

	template <class NodePtr>
	NodePtr tree_max(NodePtr x)
	{
		while (x->right != nullptr)
			x = x->right;
		return x;
	}

	template <class NodePtr>
	NodePtr tree_next(NodePtr x)
	{
		if (x->right != nullptr)
			return tree_min(x->right);
		while (!tree_is_left_child(x))
			x = x->parent;
		return x->parent;
	}

	template <class NodePtr>
	NodePtr tree_next_iter(NodePtr x)
	{
		if (x->right != nullptr)
			return tree_min(x->right);
		while (!tree_is_left_child(x))
			x = x->parent;
		return x->parent;
	}

	template <class NodePtr>
	NodePtr tree_prev_iter(NodePtr x)
	{
		if (x->left != nullptr)
			return tree_max(x->left);
		NodePtr xx = x;
		while (tree_is_left_child(xx))
			xx = xx->parent;
		return xx->parent;
	}

	template <class NodePtr>
	NodePtr tree_leaf(NodePtr x)
	{
		while (true)
		{
			if (x->left != nullptr)
			{
				x = x->left;
				continue;
			}
			if (x->right != nullptr)
			{
				x = x->right;
				continue;
			}
			break;
		}
		return x;
	}

	template <class NodePtr>
	void tree_left_rotate(NodePtr x)
	{
		NodePtr y = x->right;
		x->right = y->left;
		if (x->right != nullptr)
			x->right->parent = x;
		y->parent = x->parent;
		if (tree_is_left_child(x))
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	template <class NodePtr>
	void tree_right_rotate(NodePtr x)
	{
		NodePtr y = x->left;
		x->left = y->right;
		if (x->left != nullptr)
			x->left->parent = x;
		y->parent = x->parent;
		if (tree_is_left_child(x))
			x->parent->left = y;
		else
			x->parent->right = y;
		y->right = x;
		x->parent = y;
	}

	template <class NodePtr>
	void tree_balance_after_insert(NodePtr root, NodePtr x)
	{
		x->is_black = x == root;
		while (x != root && !x->parent->is_black)
		{
			if (tree_is_left_child(x->parent))
			{
				NodePtr y = x->parent->parent->right;
				if (y != nullptr && !y->is_black)
				{
					x = x->parent;
					x->is_black = true;
					x = x->parent;
					x->is_black = x == root;
					y->is_black = true;
				}
				else
				{
					if (!tree_is_left_child(x))
					{
						x = x->parent;
						tree_left_rotate(x);
					}
					x = x->parent;
					x->is_black = true;
					x = x->parent;
					x->is_black = false;
					tree_right_rotate(x);
					break;
				}
			}
			else
			{
				NodePtr y = x->parent->parent->left;
				if (y != nullptr && !y->is_black)
				{
					x = x->parent;
					x->is_black = true;
					x = x->parent;
					x->is_black = x == root;
					y->is_black = true;
				}
				else
				{
					if (tree_is_left_child(x))
					{
						x = x->parent;
						tree_right_rotate(x);
					}
					x = x->parent;
					x->is_black = true;
					x = x->parent;
					x->is_black = false;
					tree_left_rotate(x);
					break;
				}
			}
		}
	}

	template <class NodePtr>
	void tree_remove(NodePtr root, NodePtr z)
	{
		NodePtr y = (z->left == nullptr || z->right == nullptr) ? z : tree_next(z);
		NodePtr x = y->left != nullptr ? y->left : y->right;
		NodePtr w = nullptr;
		if (x != nullptr)
			x->parent = y->parent;
		if (tree_is_left_child(y))
		{
			y->parent->left = x;
			if (y != root)
				w = y->parent->right;
			else
				root = x;
		}
		else
		{
			y->parent->right = x;
			w = y->parent->left;
		}
		bool removed_black = y->is_black;
		if (y != z)
		{
			y->parent = z->parent;
			if (tree_is_left_child(z))
				y->parent->left = y;
			else
				y->parent->right = y;
			y->left = z->left;
			y->left->parent = y;
			y->right = z->right;
			if (y->right != nullptr)
				y->right->parent = y;
			y->is_black = z->is_black;
			if (root == z)
				root = y;
		}
		if (removed_black && root != nullptr)
		{
			if (x != nullptr)
				x->is_black = true;
			else
			{
				while (true)
				{
					if (!tree_is_left_child(w))
					{
						if (!w->is_black)
						{
							w->is_black = true;
							w->parent->is_black = false;
							tree_left_rotate(w->parent);
							if (root == w->left)
								root = w;
							w = w->left->right;
						}
						if ((w->left == nullptr || w->left->is_black) && (w->right == nullptr || w->right->is_black))
						{
							w->is_black = false;
							x = w->parent;
							if (x == root || !x->is_black)
							{
								x->is_black = true;
								break;
							}
							w = tree_is_left_child(x) ? x->parent->right : x->parent->left;
						}
						else
						{
							if (w->right == nullptr || w->right->is_black)
							{
								w->left->is_black = true;
								w->is_black = false;
								tree_right_rotate(w);
								w = w->parent;
							}
							w->is_black = w->parent->is_black;
							w->parent->is_black = true;
							w->right->is_black = true;
							tree_left_rotate(w->parent);
							break;
						}
					}
					else
					{
						if (!w->is_black)
						{
							w->is_black = true;
							w->parent->is_black = false;
							tree_right_rotate(w->parent);
							if (root == w->right)
								root = w;
							w = w->right->left;
						}
						if ((w->left == nullptr || w->left->is_black) && (w->right == nullptr || w->right->is_black))
						{
							w->is_black = false;
							x = w->parent;
							if (!x->is_black || x == root)
							{
								x->is_black = true;
								break;
							}
							w = tree_is_left_child(x) ? x->parent->right : x->parent->left;
						}
						else
						{
							if (w->left == nullptr || w->left->is_black)
							{
								w->right->is_black = true;
								w->is_black = false;
								tree_left_rotate(w);
								w = w->parent;
							}
							w->is_black = w->parent->is_black;
							w->parent->is_black = true;
							w->left->is_black = true;
							tree_right_rotate(w->parent);
							break;
						}
					}
				}
			}
		}
	}

	template <class T>
	struct tree_node
	{
		T			content;
		tree_node*	parent;
		tree_node*	right;
		tree_node*	left;
		bool		is_black;

		tree_node(const T& x = T()) : content(x), parent(nullptr), right(nullptr), left(nullptr), is_black(false) {}
	};

	template <class Key, class T, class KeyOfValue, class Compare, class Alloc>
	class RBTree
	{
	public:
		typedef Key			key_type;
		typedef T			value_type;
		typedef Compare		key_compare;
		typedef Alloc		allocator_type;
		typedef ptrdiff_t	difference_type;
		typedef size_t		size_type;

		typedef tree_node<T>	node_type;
		typedef node_type*		NodePtr;
		
		typedef tree_iter<value_type, NodePtr>			iterator;
		typedef tree_iter<const value_type, NodePtr>	const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		typedef typename Alloc::rebind<node_type>::other	NodeAllocator;
	private:
		static const key_type& get_key(NodePtr x) { return KeyOfValue()(x->content); }

		NodePtr create_node(const value_type& x)
		{
			NodePtr tmp = this->_alloc.allocate(1);
			this->_alloc.construct(&tmp->content, x);
			return tmp;
		}

		NodePtr clone_node(NodePtr root)
		{
			if (root != nullptr)
			{
				NodePtr curr_node = create_node(root->content);
				curr_node->left = this->clone_node(root->left);
				curr_node->right = this->clone_node(root->right);
				return curr_node;
			}
			return nullptr;
		}

		iterator _lower_bound(NodePtr x, NodePtr y, const key_type& k)
		{
			while (x != nullptr)
			{
				if (!this->_comp(get_key(x), k))
				{
					y = x;
					x = x->left;
				}
				else
					x = x->right;
			}
			return iterator(y);
		}

		NodePtr		_root;
		key_compare	_comp;
		size_type	_node_count;
		Alloc		_alloc;
	public:
		RBTree(const value_compare& comp, const allocator_type& a = allocator_type()) : _root(), _comp(comp), _node_count(0), _alloc(NodeAllocator(a)) {}
		RBTree(const RBTree& other)
			: _comp(other._comp), _alloc(other._alloc), _node_count(other._node_count)
		{
			this->_root = clone_node(other._root);
		}

		iterator				begin()			{ return iterator(tree_min(this->_root)); }
		const_iterator			begin() const	{ return const_iterator(tree_min(this->_root)); }
		iterator				end()			{ return iterator(tree_max(this->_root)); }
		const_iterator			end() const		{ return const_iterator(tree_max(this->_root)); }
		reverse_iterator		rbegin()		{ return reverse_iterator(this->end()); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(this->end()); }
		reverse_iterator		rend()			{ return reverse_iterator(this->begin()); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(this->begin()); }

		bool 		empty() const		{ return this->_node_count == 0; }
		size_type	size() const		{ return this->_node_count; }
		size_type	max_size() const	{ return this->_alloc.max_size(); }

		template <class InputIterator>
		void insert_unique(InputIterator first, InputIterator last)
		{

		}

		iterator find(const key_type& k)
		{
			iterator j = this->_lower_bound(this->begin(), this->end(), k);
			return (j == this->end() || this->_comp(k, get_key(*j))) ? this->end() : j;
		}

		// const_iterator find(const key_type& k)
		// {
		// 	const_iterator j = this->_lower_bound(this->begin(), this->end(), k);

		// }
	};
}

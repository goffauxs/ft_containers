#pragma once
#include <memory>
#include "utils.hpp"
#include "pair.hpp"

namespace ft
{
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

	template <class EndNodePtr, class NodePtr>
	EndNodePtr tree_next_iter(NodePtr x)
	{
		if (x->right != nullptr)
			return static_cast<EndNodePtr>(tree_min(x->right));
		while (!tree_is_left_child(x))
			x = x->parent;
		return static_cast<EndNodePtr>(x->parent);
	}

	template <class NodePtr, class EndNodePtr>
	NodePtr tree_prev_iter(EndNodePtr x)
	{
		if (x->left != nullptr)
			return tree_max(x->left);
		NodePtr xx = static_cast<NodePtr>(x);
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
			x->right->set_parent(x);
		y->parent = x->parent;
		if (tree_is_left_child(x))
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->set_parent(y);
	}

	template <class NodePtr>
	void tree_right_rotate(NodePtr x)
	{
		NodePtr y = x->left;
		x->left = y->right;
		if (x->left != nullptr)
			x->left->set_parent(x);
		y->parent = x->parent;
		if (tree_is_left_child(x))
			x->parent->left = y;
		else
			x->parent->right = y;
		y->right = x;
		x->set_parent(y);
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
			y->left->set_parent(y);
			y->right = z->right;
			if (y->right != nullptr)
				y->right->set_parent(y);
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
		T	content;
		tree_node*	parent;
		tree_node*	right;
		tree_node*	left;

		tree_node(const T& x = T()) : content(x), parent(nullptr), right(nullptr), left(nullptr) {}
	};

	template <class Key, class T, class Compare, class Alloc>
	class RBTree
	{
	public:
		typedef T		value_type;
		typedef Compare	value_compare;
		typedef Alloc	allocator_type;
	private:
		typedef 
	}
}

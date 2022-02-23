#pragma once
#include <memory>
#include "utils.hpp"
#include "pair.hpp"

namespace ft
{
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

	enum RB_tree_color { red = false, black = true };

	template <class T, class Compare, class Alloc>
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

#pragma once
#include "RBTree.hpp"

namespace ft
{
	static tree_node_base* local_tree_increment(tree_node_base* x)
	{
		if (x->right != nullptr)
		{
			x = x->right;
			while (x->left != nullptr)
				x = x->left;
		}
		else
		{
			tree_node_base* y = x->parent;
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

			tree_node_base* tree_increment(tree_node_base* x)		{ return local_tree_increment(x); }
	const	tree_node_base* tree_increment(const tree_node_base* x)	{ return local_tree_increment(const_cast<tree_node_base*>(x)); }

	static tree_node_base* local_tree_decrement(tree_node_base* x)
	{
		if (!x->is_black && x->parent->parent == x)
			x = x->right;
		else if (x->left != nullptr)
		{
			tree_node_base* y = x->left;
			while (y->right != nullptr)
				y = y->right;
			x = y;
		}
		else
		{
			tree_node_base* y = x->parent;
			while (x == y->left)
			{
				x = y;
				y = y->parent;
			}
			x = y;
		}
		return x;
	}

			tree_node_base* tree_decrement(tree_node_base* x)		{ return local_tree_decrement(x); }
	const	tree_node_base* tree_decrement(const tree_node_base* x)	{ return local_tree_decrement(const_cast<tree_node_base*>(x)); }
}
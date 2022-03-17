#ifndef TREE_UTILS_H
# define TREE_UTILS_H

#include <iostream>

namespace ft
{
	struct tree_node_base
	{
		typedef tree_node_base*			base_ptr;
		typedef const tree_node_base*	const_base_ptr;

		bool		is_black;
		base_ptr	parent;
		base_ptr	left;
		base_ptr	right;

		static base_ptr _min(base_ptr x)
		{
			while (x->left != nullptr) x = x->left;
			return x;
		}

		static const_base_ptr _min(const_base_ptr x)
		{
			while (x->left != nullptr) x = x->left;
			return x;
		}

		static base_ptr _max(base_ptr x)
		{
			while (x->right != nullptr) x = x->right;
			return x;
		}

		static const_base_ptr _max(const_base_ptr x)
		{
			while (x->right != nullptr) x = x->right;
			return x;
		}
	};

	template <typename Key_compare>
	struct tree_key_compare
	{
		Key_compare key_compare;

		tree_key_compare() : key_compare() {}
		tree_key_compare(const Key_compare& comp) : key_compare(comp) {}
	};

	struct tree_header
	{
		tree_node_base	header;
		size_t			node_count;

		tree_header()
		{
			header.is_black = false;
			reset();
		}

		void move_data(tree_header& from)
		{
			header.is_black = from.header.is_black;
			header.parent = from.header.parent;
			header.left = from.header.left;
			header.right = from.header.right;
			header.parent->parent = &header;
			node_count = from.node_count;

			from.reset();
		}

		void reset()
		{
			header.parent = nullptr;
			header.left = &header;
			header.right = &header;
			node_count = 0;
		}
	};

	template <class T>
	struct tree_node : public tree_node_base
	{
		typedef tree_node<T>* link_type;

		T value_field;

				T* valptr()			{ return std::addressof(value_field); }
		const	T* valptr() const	{ return std::addressof(value_field); }
	};

	static tree_node_base* ft_tree_increment(tree_node_base* x)
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

			tree_node_base* tree_increment(tree_node_base* x)		{ return ft_tree_increment(x); }
	const	tree_node_base* tree_increment(const tree_node_base* x)	{ return ft_tree_increment(const_cast<tree_node_base*>(x)); }

	static tree_node_base* ft_tree_decrement(tree_node_base* x)
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

			tree_node_base* tree_decrement(tree_node_base* x)		{ return ft_tree_decrement(x); }
	const	tree_node_base* tree_decrement(const tree_node_base* x)	{ return ft_tree_decrement(const_cast<tree_node_base*>(x)); }

	static void ft_tree_rotate_left(tree_node_base* const x, tree_node_base*& root)
	{
		tree_node_base* const y = x->right;
		
		x->right = y->left;
		if (y->left != nullptr)
			y->left->parent = x;
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	static void ft_tree_rotate_right(tree_node_base* const x, tree_node_base*& root)
	{
		tree_node_base* const y = x->left;

		x->left = y->right;
		if (y->right != nullptr)
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

	void tree_insert_and_rebalance(const bool insert_left, tree_node_base* x, tree_node_base* p, tree_node_base& header)
	{
		tree_node_base *& root = header.parent;

		// Insert
		x->parent = p;
		x->left = nullptr;
		x->right = nullptr;
		x->is_black = false;

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

		// Rebalance
		while (x != root && !x->parent->is_black)
		{
			tree_node_base* const gp = x->parent->parent;

			if (x->parent == gp->left)
			{
				tree_node_base* const u = gp->right;
				if (u != nullptr && !u->is_black)
				{
					x->parent->is_black = true;
					u->is_black = true;
					gp->is_black = false;
					x = gp;
				}
				else
				{
					if (x == x->parent->right)
					{
						x = x->parent;
						ft_tree_rotate_left(x, root);
					}
					x->parent->is_black = true;
					gp->is_black = false;
					ft_tree_rotate_right(gp, root);
				}
			}
			else
			{
				tree_node_base* const y = gp->left;
				if (y && !y->is_black)
				{
					x->parent->is_black = true;
					y->is_black = true;
					gp->is_black = false;
					x = gp;
				}
				else
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						ft_tree_rotate_right(x, root);
					}
					x->parent->is_black = true;
					gp->is_black = false;
					ft_tree_rotate_left(gp, root);
				}
			}
		}
		root->is_black = true;
	}

	tree_node_base* tree_rebalance_for_erase(tree_node_base* const z, tree_node_base& header)
	{
		tree_node_base*& root = header.parent;
		tree_node_base*& leftmost = header.left;
		tree_node_base*& rightmost = header.right;
		tree_node_base* y = z;
		tree_node_base* x = nullptr;
		tree_node_base* x_parent = nullptr;

		if (y->left == nullptr)
			x = y->right;
		else
		{
			if (y->right == 0)
				x = y->left;
			else
			{
				y = y->right;
				while (y->left != nullptr)
					y = y->left;
				x = y->right;
			}
		}

		if (y != z)
		{
			z->left->parent = y;
			y->left = z->left;
			if (y != z->right)
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
			std::swap(y->is_black, z->is_black);
			y = z;
		}
		else
		{
			x_parent = y->parent;
			if (x) x->parent = y->parent;
			if (root == z)
				root = x;
			else
			{
				if (z->parent->left == z)
					z->parent->left = x;
				else
					z->parent->right = x;
			}
			if (leftmost == z)
			{
				if (z->right == nullptr)
					leftmost = z->parent;
				else
					leftmost = tree_node_base::_min(x);
			}
			if (rightmost == z)
			{
				if (z->left == nullptr)
					rightmost = z->parent;
				else
					rightmost = tree_node_base::_max(x);
			}
		}

		if (y->is_black)
		{
			while (x != root && (x == nullptr || x->is_black))
			{
				if (x == x_parent->left)
				{
					tree_node_base* w = x_parent->right;
					if (!w->is_black)
					{
						w->is_black = true;
						x_parent->is_black = false;
						ft_tree_rotate_left(x_parent, root);
						w = x_parent->right;
					}
					if ((w->left == nullptr || w->left->is_black) && (w->right == nullptr || w->right->is_black))
					{
						w->is_black = false;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else
					{
						if (w->right == nullptr || w->right->is_black)
						{
							w->left->is_black = true;
							w->is_black = false;
							ft_tree_rotate_right(w, root);
							w = x_parent->right;
						}
						w->is_black = x_parent->is_black;
						x_parent->is_black = true;
						if (w->right)
							w->right->is_black = true;
						ft_tree_rotate_left(x_parent, root);
						break;
					}
				}
				else
				{
					tree_node_base* w = x_parent->left;
					if (!w->is_black)
					{
						w->is_black = true;
						x_parent->is_black = false;
						ft_tree_rotate_right(x_parent, root);
						w = x_parent->left;
					}
					if ((w->right == nullptr || w->right->is_black) && (w->left == nullptr || w->left->is_black))
					{
						w->is_black = false;
						x = x_parent;
						x_parent = x_parent->parent;
					}
					else
					{
						if (w->left == nullptr || w->left->is_black)
						{
							w->right->is_black = true;
							w->is_black = false;
							ft_tree_rotate_left(w, root);
							w = x_parent->left;
						}
						w->is_black = x_parent->is_black;
						x_parent->is_black = true;
						if (w->left)
							w->left->is_black = true;
						ft_tree_rotate_right(x_parent, root);
						break;
					}
				}
			}
			if (x) x->is_black = true;
		}
		return y;
	}
}


#endif /* TREE_UTILS_H */

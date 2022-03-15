#pragma once
#include <memory>
#include "reverse_iterator.hpp"
#include "utils.hpp"
#include "pair.hpp"

namespace ft
{
	template <class T>
	class tree_iter
	{
		typedef T	value_type;
		typedef T&	reference;
		typedef T*	pointer;

		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;

		typedef tree_iter<T>				self;
		typedef tree_node_base::base_ptr	base_ptr;
		typedef tree_node<T>*				link_type;

					tree_iter() : _node() {}
		explicit	tree_iter(base_ptr x) : _node(x) {}

		reference operator*() const { return *static_cast<link_type>(_node)->valptr(); }
		pointer operator->() const { return static_cast<link_type>(_node)->valptr(); }

		self& operator++() { _node = tree_}
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

	struct tree_node_base
	{
		typedef tree_node_base*			base_ptr;
		typedef const tree_node_base*	const_base_ptr;

		bool		is_black;
		base_ptr	parent;
		base_ptr	left;
		base_ptr	right;
	};

	template <typename Key_compare>
	struct tree_key_compare
	{
		Key_compare key_compare;

		tree_key_compare() : key_compare() {}
		tree_key_compare(const Key_compare& comp) : key_compare(comp) {}
	};

	template <class T>
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
		tree_node(const T& x = T()) : content(x), parent(nullptr), right(nullptr), left(nullptr), is_black(false) {}
	};

	template <class Key, class T, class KeyOfValue, class Compare, class Alloc>
	class RBTree
	{
		typedef typename Alloc::rebind<tree_node<T> >::other	node_allocator;
	protected:
		typedef tree_node_base*			base_ptr;
		typedef const tree_node_base*	const_base_ptr;
		typedef tree_node<T>*			link_type;
		typedef const tree_node<T>*		const_link_type;
	private:
		struct reuse_or_alloc_node
		{
			reuse_or_alloc_node(RBTree& t)
				: _root(t._root()), _nodes(t.rightmost()), _t(t)
			{
				if (_root)
				{
					_root->parent = nullptr;
					if (_nodes->left)
						_nodes = nodes->left;
				}
				else
					_nodes = nullptr;
			}

			~reuse_or_alloc_node() { _t.erase(static_cast<link_type>(_root)); }

			template <typename Arg>
			link_type operator()(const Arg& arg)
			{
				link_type node = static_case<link_type>(extract());
				if (node)
				{
					_t.destroy_node(node);
					_t.construct_node(node, arg);
					return node;
				}
				return _t.create_node(arg);
			}
		private:
			base_ptr extract()
			{
				if (!_nodes)
					return _nodes;
				base_ptr node = _nodes;
				_nodes = _nodes->parent;
				if (_nodes)
				{
					if (_nodes->right == node)
					{
						_nodes->right = nullptr;
						if (_nodes->left)
						{
							_nodes = _nodes->left;
							while (_nodes->right)
								_nodes = _nodes->right;
							if (_nodes->left)
								_nodes = _nodes->left;
						}
					}
					else
						_nodes->left = nullptr;
				}
				else
					_root = nullptr;
				return node;
			}

			base_ptr	_root;
			base_ptr	_nodes;
			RBTree&		_t;
		};

		struct Alloc_node
		{
			Alloc_node(RBTree& t) : _t(t) {}

			template <typename Arg>
			link_type operator()(const Arg& arg) const { return _t.create_node(arg); }
		private:
			RBTree& _t;
		};
	public:
		typedef Key					key_type;
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_pointer;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		// typedef Compare		key_compare;
		typedef Alloc		allocator_type;
		typedef ptrdiff_t	difference_type;
		typedef size_t		size_type;

				node_allocator&	get_node_allocator()		{ return *static_cast<node_allocator*>(&this->impl); }
		const	node_allocator&	get_node_allocator() const	{ return *static_cast<const node_allocator*>(&this->impl); }
				allocator_type	get_allocator() const		{ return allocator_type(get_node_allocator()); }

		// typedef tree_node<T>	node_type;
		// typedef node_type*		NodePtr;
		
		// typedef tree_iter<value_type, NodePtr>			iterator;
		// typedef tree_iter<const value_type, NodePtr>	const_iterator;
		// typedef ft::reverse_iterator<iterator>			reverse_iterator;
		// typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
	protected:
		link_type	get_node()				{ return get_node_allocator().allocate(1); }
		void		put_node(link_type p)	{ get_node_allocator().deallocate(p, 1); }
		void		construct_node(link_type node, const value_type& x)
		{
			try
			{
				get_allocator().construct(node->valptr(), x);
			}
			catch (const std::exception& e)
			{
				put_node(node);
				std::cerr << e.what() << std::endl;
			}
		}

		link_type create_node(const value_type& x)
		{
			link_type tmp = get_node();
			construct_node(tmp, x);
			return tmp;
		}

		void destroy_node(link_type p) { get_allocator().destroy(p->valptr()); }

		void drop_node(link_type p)
		{
			destroy_node(p);
			put_node(p);
		}

		template <typename NodeGen>
		link_type clone_node(const_link_type x, NodeGen& node_gen)
		{
			link_type tmp = node_gen(*x->valptr());
			tmp->is_black = x->is_black;
			tmp->left = nullptr;
			tmp->right = nullptr;
			return tmp;
		}
	protected:
		template <typename Key_compare>
		struct tree_impl : public node_allocator, public tree_key_compare<Key_compare>, public tree_header
		{
			typedef tree_key_compare<Key_compare> base_key_compare;

			tree_impl() {}
			tree_impl(const tree_impl& x) : base_key_compare(x.key_compare) {}
			tree_impl(const tree_impl& comp, const node_allocator& a) : node_allocator(a), base_key_compare(x.key_compare) {}
		};

		tree_impl<Compare> impl;
	protected:
		base_ptr&		_root()				{ return this->impl.header.parent; }
		const_base_ptr	_root() const		{ return this->impl.header.parent; }
		base_ptr&		_leftmost()			{ return this->impl.header.left; }
		const_base_ptr _leftmost() const	{ return this->impl.header.left; }
		bases_ptr&		_rightmost()		{ return this->impl.header.right; }
		const_base_ptr _rightmost() const	{ return this->impl.header.right; }
		link_type		_begin()			{ return static_cast<link_type>(this->impl.header.parent); }
		const_link_type _begin() const		{ return static_cast<const_link_type>(this->impl.header.parent); }
		base_ptr		_end()				{ return &this->impl.header; }
		const_base_ptr	_end() const		{ return &this->impl.header; }
		
		static const_reference	_value(const_link_type x) { return *x->valptr(); }
		static const Key&		_key(const_link_type x) { return KeyOfValue()(_value(x)); }
		static link_type		_left(base_ptr x) { return static_cast<link_type>(x->left); }
		static const_link_type	_left(const_base_ptr x) { return static_cast<const_link_type>(x->left); }
		static link_type		_right(base_ptr x) { return static_cast<link_type>(x->right); }
		static const_link_type	_right(const_base_ptr x) { return static_cast<const_link_type>(x->right); }
		static const_reference	_value(const_base_ptr x) { return *static_cast<const_link_type>(x)->valptr(); }
		static const Key&		_key(const_base_ptr x) { return KeyOfValue()(_value(x)); }
	public:
		typedef tree_iter<
	private:
		static const key_type& _get_key(NodePtr x) { return KeyOfValue()(x->content); }

		ft::pair<NodePtr, NodePtr> _get_insert_unique_pos(const key_type& k)
		{
			NodePtr x = this->_root;
			NodePtr y = end()._ptr;
			bool comp = true;
			while (x != nullptr)
			{
				y = x;
				comp = this->_comp(k, _get_key(x));
				x = comp ? x->left : x->right;
			}
			iterator j = iterator(y);
			if (comp)
			{
				if (j == begin())
					return ft::make_pair(x, y);
				else
					--j;
			}
			if (this->_comp(_get_key(j._ptr), k))
				return ft::make_pair(x, y);
			return ft::make_pair(j._ptr, nullptr);
		}

		NodePtr _create_node(const value_type& x)
		{
			NodePtr tmp = this->_alloc.allocate(1);
			this->_alloc.construct(&tmp->content, x);
			return tmp;
		}

		NodePtr _clone_node(NodePtr root)
		{
			if (root != nullptr)
			{
				NodePtr curr_node = _create_node(root->content);
				curr_node->left = _clone_node(root->left);
				curr_node->right = _clone_node(root->right);
				return curr_node;
			}
			return nullptr;
		}

		iterator _lower_bound(NodePtr x, NodePtr y, const key_type& k)
		{
			while (x != nullptr)
			{
				if (!this->_comp(_get_key(x), k))
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
		RBTree(const value_compare& comp, const allocator_type& a = allocator_type()) : _root(), _comp(comp), _node_count(0), _alloc(node_allocator(a)) {}
		RBTree(const RBTree& other)
			: _comp(other._comp), _alloc(other._alloc), _node_count(other._node_count)
		{
			this->_root = _clone_node(other._root);
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

		ft::pair<iterator, bool> insert_unique(const value_type& v)
		{
			ft::pair<iterator, bool> res;
			res.second = find(_get_key(v)) == this->end() ? false : true;
			if (ret.second)
			{

			}
		}

		template <class InputIterator>
		void insert_unique(InputIterator first, InputIterator last)
		{

		}

		iterator find(const key_type& k)
		{
			iterator j = _lower_bound(begin(), end(), k);
			return (j == end() || this->_comp(k, _get_key(j._ptr))) ? end() : j;
		}

		const_iterator find(const key_type& k) const
		{
			const_iterator j = _lower_bound(begin(), end(), k);
			return (j == end() || this->_comp(k, _get_key(j._ptr))) ? end() : j;
		}
	};
}

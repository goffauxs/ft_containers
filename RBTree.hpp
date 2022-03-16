#ifndef RBTREE_H
# define RBTREE_H
# include <memory>
# include "reverse_iterator.hpp"
# include "utils.hpp"
# include "tree_utils.hpp"
# include "pair.hpp"

namespace ft
{
	template <class T>
	struct tree_iter
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

		self& operator++() { _node = tree_increment(_node); return *this; }
		self operator++(int)
		{
			self tmp = *this;
			_node = tree_increment(_node);
			return tmp;
		}
		self& operator--() { _node = tree_decrement(_node); return *this; }
		self operator--(int)
		{
			self tmp = *this;
			_node = tree_decrement(_node);
			return tmp;
		}

		bool operator==(const self& x) const { return _node == x._node; }
		bool operator!=(const self& x) const { return _node != x._node; }

		base_ptr _node;
	};

	template <class T>
	struct const_tree_iter
	{
		typedef T	value_type;
		typedef T&	reference;
		typedef T*	pointer;

		typedef tree_iter<T>	iterator;

		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;

		typedef const_tree_iter<T>				self;
		typedef	tree_node_base::const_base_ptr	base_ptr;
		typedef const tree_node<T>*				link_type;

					const_tree_iter() : _node() {}
		explicit	const_tree_iter(base_ptr x) : _node(x) {}
					const_tree_iter(const iterator& it) : _node(it._node) {}
		
		iterator _const_cast() const { return iterator(const_cast<typename iterator::base_ptr>(_node)); }
		
		reference operator*() const { return *static_cast<link_type>(_node)->valptr(); }
		pointer operator->() const { return static_cast<link_type>(_node)->valptr(); }

		self& operator++()		{ _node = tree_increment(_node); return *this; }
		self operator++(int)	{ self tmp = *this; _node = tree_increment(_node); return tmp; }
		self& operator--()		{ _node = tree_decrement(_node); return *this; }
		self operator--(int)	{ self tmp = *this; _node = tree_decrement(_node); return tmp; }

		bool operator==(const self& x) const { return _node == x._node; }
		bool operator!=(const self& x) const { return _node != x._node; }

		base_ptr _node;
	};

	template <typename T>
	bool operator==(const tree_iter<T>& x, const const_tree_iter<T>& y) { return x._node == y._node; }

	template <typename T>
	bool operator!=(const tree_iter<T>& x, const const_tree_iter<T>& y) { return x._node != y._node; }

	template <class NodePtr>
	bool tree_is_left_child(NodePtr x) { return x == x->parent->left; }

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
		typedef tree_iter<value_type>		iterator;
		typedef const_tree_iter<value_type>	const_iterator;

		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		ft::pair<base_ptr, base_ptr> get_insert_unique_pos(const key_type& k)
		{
			typedef ft::pair<base_ptr, base_ptr> res;
			link_type x = _begin();
			base_ptr y = _end();
			bool comp = true;
			while (x != nullptr)
			{
				y = x;
				comp = impl.key_compare(k, _key(x));
				x = comp ? _left(x) : _right(x);
			}
			iterator j = iterator(y);
			if (comp)
			{
				if (j == begin())
					return res(x, y);
				else
					--j;
			}
			if (impl.key_compare(_key(j._node), k))
				return res(x, y);
			return res(j._node, nullptr);
		}

		ft::pair<base_ptr, base_ptr> get_insert_equal_pos(const key_type& k)
		{
			typedef ft::pair<base_ptr, base_ptr> res;
			link_type x = _begin();
			base_ptr y = _end();
			while (x != nullptr)
			{
				y = x;
				x = impl.key_compare(k, _key(x)) ? _left(x) : _right(x);
			}
			return res(x, y);
		}
	private:
		// Modifiers
		template <typename node_gen>
		iterator insert(base_ptr x, base_ptr p, const value_type& v, node_gen&)
		{
			bool insert_left = (x != nullptr || p == _end() || impl.key_compare(KeyOfValue()(v), _key(p)));
			link_type z = node_gen(v);

			tree_insert_and_rebalance(insert_left, z, p, this->impl.header);
			++impl.node_count;
			return iterator(z);
		}

		iterator insert_lower(base_ptr y, const value_type& v)
		{
			bool insert_left = (p == _end() || !impl.key_compare(_key(p), KeyOfValue()(v)));
			link_type z = create_node(v);

			tree_insert_and_rebalance(insert_left, z, p, this->impl.header);
			++impl.node_count;
			return iterator(z);
		}

		iterator insert_equal_lower(const value_type& v)
		{
			link_type x = _begin();
			base_ptr y = _end();
			while (x != nullptr)
			{
				y = x;
				x = !impl.key_compare(_key(x), KeyOfValue()(v)) ? _left(x) : _right(x);
			}
			return insert_lower(y, v);
		}

		template <typename node_gen>
		link_type copy(const_link_type x, base_ptr p, node_gen&)
		{
			link_type top = clone_node(x, node_gen);
			top->parent = p;

			try
			{
				if (x->right)
					top->right = copy(_right(x), top, node_gen);
				p = top;
				x = _left(x);

				while (x != nullptr)
				{
					link_type y = clone_node(x, node_gen);
					p->left = y;
					y->parent = p;
					if (x->right)
						y->right = copy(_right(x), y, node_gen);
					p = y;
					x = _left(x);
				}
			}
			catch (const std::exception& e)
			{
				erase(top);
				std::cerr << e.what() << std::endl;
			}
			return top;
		}

		template <typename node_gen>
		link_type copy(const RBTree& x, node_gen& gen)
		{
			link_type root = copy(x._begin(), _end(), gen);
			_leftmost() = _min(root);
			_rightmost() = _max(root);
			impl.node_count = x.impl.node_count;
			return root;
		}

		link_type copy(const RBTree& x)
		{
			Alloc_node an(*this);
			return copy(x, an);
		}

		void erase(link_type x)
		{
			while (x != nullptr)
			{
				erase(_right(x));
				link_type y = _left(x);
				drop_node(x);
				x = y;
			}
		}

		iterator lower_bound(link_type x, base_ptr y, const Key& k)
		{
			while (x != nullptr)
			{
				if (!impl.key_compare(_key(x, k)))
				{
					y = x;
					x = _left(x);
				}
				else
					x = _right(x);
			}
			return iterator(y);
		}

		const_iterator lower_bound(const_link_type x, const_base_ptr y, const Key& k)
		{
			while (x != nullptr)
			{
				if (!impl.key_compare(_key(x), k))
				{
					y = x;
					x = _left(x);
				}
				else
					x = _right(x);
			}
			return const_iterator(y);
		}

		iterator upper_bound(link_type x, base_ptr y, const Key& k)
		{
			while (x != nullptr)
			{
				if (impl.key_compare(_key(x), k))
				{
					y = x;
					x = _left(x);
				}
				else
					x = _right(x);
			}
			return iterator(y);
		}

		const_iterator upper_bound(const_link_type x, const_base_ptr y, const Key& k)
		{
			while (x != nullptr)
			{
				if (impl.key_compare(_key(x), k))
				{
					y = x;
					x = _left(x);
				}
				else
					x = _right(x);
			}
			return const_iterator(y);
		}
	public:
		RBTree() {}
		RBTree(const Compare& comp, const allocator_type& a = allocator_type()) : impl(comp, node_allocator(a)) {}
		RBTree(const RBTree& other)
			: impl(other.impl)
		{
			if (x._root() != nullptr)
				_root() = copy(other);
		}
		~RBTree() { erase(_begin()); }

		RBTree& operator=(const RBTree& rhs)
		{
			if (this != &rhs)
			{
				reuse_or_alloc_node roan(*this);
				impl.reset();
				impl.key_compare = rhs.impl.key_compare;
				if (rhs._root() != nullptr)
					_root() = copy(rhs, roan);
			}
			return *this;
		}

		Compare key_comp() const { return impl.key_compare; }
		iterator				begin()			{ return iterator(this->impl.header.left); }
		const_iterator			begin() const	{ return const_iterator(this->impl.header.left); }
		iterator				end()			{ return iterator(&this->impl.header); }
		const_iterator			end() const		{ return const_iterator(&this->impl.header); }
		reverse_iterator		rbegin()		{ return reverse_iterator(end()); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(end()); }
		reverse_iterator		rend()			{ return reverse_iterator(begin()); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(begin()); }

		bool 		empty() const		{ return impl.node_count == 0; }
		size_type	size() const		{ return impl.node_count; }
		size_type	max_size() const	{ return get_node_allocator().max_size(); }

		void swap(RBTree& other)
		{
			if (_root() == nullptr)
			{
				if (other._root() != nullptr)
					impl.move_data(other.impl);
			}
			else if (other._root() == nullptr)
				other.impl.move_data(impl);
			else
			{
				std::swap(_root(), other._root());
				std::swap(_leftmost(), other._leftmost());
				std::swap(_rightmost(), other._rightmost());

				_root()->parent = _end();
				other._root()->parent = other._end();
				std::swap(this->impl.node_count, other.impl.node_count);
			}
			std::swap(this->impl.key_compare, other.impl.key_compare);
			std::swap(get_node_allocator(), other.get_node_allocator());
		}

		ft::pair<iterator, bool> insert_unique(const value_type& v)
		{
			typedef ft::pair<iterator, bool> cast;
			ft::pair<base_ptr, base_ptr> res = get_insert_unique_pos(KeyOfValue()(v));

			if (res.second)
			{
				Alloc_node an(*this);
				return cast(insert(res.first, res.second, v, an), true);
			}
			return cast(iterator(res.first), false);
		}

		iterator insert_equal(const value_type& v)
		{
			ft::pair<base_ptr, base_ptr> res = get_insert_equal_pos(KeyOfValue()(v));
			Alloc_node an(*this);
			return insert(res.first, res.second, v, an);
		}
	};
}


#endif /* RBTREE_H */

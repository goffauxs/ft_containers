#ifndef STACK_H
# define STACK_H
# include "vector.hpp"

namespace ft
{
	template <class T, class Container = ft::vector<T> >
	class stack
	{
	public:
		typedef typename Container::value_type		value_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
		typedef typename Container::size_type		size_type;
		typedef 		 Container					container_type;
	protected:
		Container c;
	public:
		// Constructor
		explicit stack(const container_type& ctnr = container_type()) : c(ctnr) {}

		// empty
		bool empty() const { return c.empty(); }

		// size
		size_type size() const { return c.size(); }

		// top
		reference		top()		{ return c.back(); }
		const_reference	top() const	{ return c.back(); }

		// push
		void push(const value_type& val) { c.push_back(val); }

		// pop
		void pop() { c.pop_back(); }

		template <class T1, class Container1>
		friend bool operator==(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs) { return lhs.c == rhs.c; }

		template <class T1, class Container1>
		friend bool operator<(const stack<T1, Container1>& lhs, const stack<T1, Container1>& rhs) { return lhs.c < rhs.c; }
	};

	template <class T, class Container>
	bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) { return !(lhs == rhs); }

	template <class T, class Container>
	bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) { return rhs < lhs; }

	template <class T, class Container>
	bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) { return !(rhs < lhs); }

	template <class T, class Container>
	bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) { return !(lhs < rhs); }
}


#endif /* STACK_H */

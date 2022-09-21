#ifndef UTILS_H
# define UTILS_H
# include "type_traits.hpp"
# include "../iterator/iterator_traits.hpp"

namespace ft
{
	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type __distance(InputIterator __first, InputIterator __last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type __n = 0;
		while(__first != __last)
		{
			++__first;
			++__n;
		}
		return __n;
	}

	template <class RandomAccessIterator>
	typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator __first, RandomAccessIterator __last, random_access_iterator_tag)
	{
		return __last - __first;
	}

	template <typename InputIterator>
	typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
	{
		return __distance(first, last, typename ft::iterator_traits<InputIterator>::iterator_category());
	}

	template <typename InputIterator>
	void __advance(InputIterator& __i, typename iterator_traits<InputIterator>::difference_type __n, input_iterator_tag)
	{
		for (; __n > 0; --__n)
			++__i;
	}

	template <typename BiDirIter>
	void __advance(BiDirIter& __i, typename iterator_traits<BiDirIter>::difference_type __n, bidirectional_iterator_tag)
	{
		if (__n >= 0)
			for (; __n > 0; --__n)
				++__i;
		else
			for (; __n < 0; ++__n)
				--__i;
	}

	template <typename RandIter>
	void __advance(RandIter& __i, typename iterator_traits<RandIter>::difference_type __n, random_access_iterator_tag)
	{
		__i += __n;
	}

	template <typename InputIterator>
	void advance(InputIterator& __i, typename iterator_traits<InputIterator>::difference_type __n)
	{
		__advance(__i, __n, typename ft::iterator_traits<InputIterator>::iterator_category());
	}

	template <class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1)
		{
			if (!(*first1 == *first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	template <class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return false;
			else if (*first1 < *first2)
				return true;
			++first1;
			++first2;
		}
		return (first2!=last2);
	}

	template <class Arg, class Result>
	struct unary_function
	{
		typedef Arg		argument_type;
		typedef Result	result_type;
	};

	template <class Arg1, class Arg2, class Result>
	struct binary_function
	{
		typedef Arg1	first_argument_type;
		typedef Arg2	second_argument_type;
		typedef Result	result_type;
	};

	template <class T>
	struct less : public binary_function<T, T, bool>
	{
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	template <typename T>
	struct Identity : public unary_function<T, T>
	{
		T& operator()(T& x) const { return x; }
		const T& operator()(const T& x) const { return x; }
	};

	template <typename T>
	struct Identity <const T> : Identity<T> {};

	template <typename Pair>
	struct Select1st : public unary_function<Pair, typename Pair::first_type>
	{
		typename Pair::first_type& operator()(Pair& x) const { return x.first; }
		const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
	};
}


#endif /* UTILS_H */

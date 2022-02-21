#include "type_traits.hpp"
#include "iterator_traits.hpp"

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
}
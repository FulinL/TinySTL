#ifndef __UNINITIALIZED_
#define __UNINITIALIZED_

#include "TinySTL_construct.h"
#include "TinySTL_iterator.h"
#include "TinySTL_type_traits.h"
#include <string>
#include <algorithm>
namespace  TinySTL
{
	//uninitialized_fill
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type)
	{
		return fill_n(first, n, x);
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type)
	{
		ForwardIterator cur = frist;
		for (; n > 0; n--, cur++)
			construct(&*cur, x);
		return cur;
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, n, x, is_POD());
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
	{
		return __uninitialized_fill_n(first, n, x, value_type(first));
	}
	
	template<class ForwardIterator, class T, class T1>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		return __uninitialized_fill_n_aux(first, last, x, is_POD());
	}

	template<class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
	{
		__uninitialized_fill(first, last, T, value_type(first));
	}

	//uninitialized_copy
	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitilaized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
	{
		while (first != last)
		{
			*result = *first;
			first++;
			result++;
		}
		return result;
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator __uninitilaized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
	{
		ForwardIterator cur = result;
		for (; first != last; first++, cur++)
			construct(&*cur, *first);
		return cur;
	}

	template<class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
	{
		typedef typename __type_traits<T>::is_POD_type is_POD;
		return __uninitilaized_copy_aux(first, last, result, is_POD());
	}

	//�ػ�char* wchar_t*�汾
	inline char * uninitialized_copy(const char * first, const char * last, char * result)
	{
		memmove(result, first, last - first);
		return result + (last - first);
	}

	inline wchar_t * uninitialized_copy(const wchar_t * first, const wchar_t * last, wchar_t * result)
	{
		memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return __uninitialized_copy(first, last, result, value_type(result));
	}
};
#endif // !__UNINITIALIZED_

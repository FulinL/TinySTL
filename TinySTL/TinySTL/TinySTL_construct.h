#ifndef __CONSTRUCT_
#define __CONSTRUCT_

#include <new>
#include "TinySTL_type_traits.h"
namespace TinySTL
{
	template<class T1, class T2>
	inline void construct(T1* p, const T2& value)
	{
		new (p)T1(value);
	}

	template<class T>
	inline void destroy(T* pointer)
	{
		pointer->~T();
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		__destroy(first, last, value_type(first));
	}

	template<class ForwardIterator, class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, T*)
	{
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first, last, trivial_destructor());
	}

	template<class ForwardIterator, class T>
	inline void __destroy(ForwardIterator first, ForwardIterator last, __false_type)
	{
		for (; first < last; first++)
		{
			destroy(&*first);
		}
	}

	template<class ForwardIterator>
	inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type){}

}
#endif
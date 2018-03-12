#include "stdafx.h"
#include "TinySTL_alloc.h"

#if 0
# include <new>
# define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
# include<iostream>
# define __THROW_BAD_ALLOC cerr<< "out of memory" << endl; exit(1);
#endif

namespace TinySTL
{
	//Ò»¼¶¿Õ¼äÅäÖÃÆ÷
	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;


	template<int inst>
	void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();
			result = malloc(n);
			if (result)
				return (result);
		}
	}
	template<int inst>
	void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
			{
				__THROW_BAD_ALLOC;
			}
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				return (result);
		}
	}
}
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
	//һ���ռ�������
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


	//�����ռ�������
	
	template<bool threads, int inst>
	char *__default_alloc_template<threads, inst>::start_free = 0;
	template<bool threads, int inst>
	char *__default_alloc_template<threads, inst>::end_free = 0;
	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;

	template<bool threads, int inst>
	typename __default_alloc_template<threads, inst>::obj * volatile __default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	
	template<bool threads, int inst>
	void * __default_alloc_template<threads, inst>::refill(size_t n)
	{
		int nobjs = 20;

		char * chunk = chunk_alloc(n, nobjs);
		obj * volatile * my_free_list;
		obj * result;
		obj * current_obj, *next_obj;
		int i;

		if (nobjs == 1)
			return (chunk);
		my_free_list = free_list + FREELIST_INDEX(n);

		result = (obj*) chunk;
		*my_free_list = next_obj = (obj *)(chunk + n);

		for (i = 1; ; i++)
		{
			current_obj = next_obj;
			next_obj = (obj *)((char *)next_obj + n);
			if (nobjs - 1 == i)
			{
				current_obj->free_list_link = 0;
				break;
			}
			else
			{
				current_obj->free_list_link = next_obj;
			}
		}
		return (result);
	}

	template<bool threads, int inst>
	char * __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int &nobjs)
	{
		char * result;
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;	//�ڴ��ʣ��ռ�

		if (bytes_left >= total_bytes)	//�ڴ��ʣ��ռ���������
		{
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else if (bytes_left >= size)	//�ڴ�ز�����ȫ�������󣬵����Թ�Ӧ����һ������
		{
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return (result);
		}
		else    //�ڴ��ʣ��ռ���һ�����鶼�޷��ṩ
		{
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);

			if (bytes_left > 0)		//�ڴ��ʣ����ͷ����ʵ��� free_list
			{
				obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);

				((obj *)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj *)start_free;
			}
			
			//����heap�ռ䣬���������ڴ��
			start_free = (char *)malloc(bytes_to_get);
			if (start_free == 0)
			{
				//heap�ռ䲻�㣬mallocʧ��
				int i;
				obj * volatile *my_free_list, *p;

				for (i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0)		//free_list������δ������
					{
						*my_free_list = p->free_list_link;
						start_free = (char *)p;
						end_free = start_free + i;
						return (chunk_alloc(size, nobjs));
					}

				}
				end_free = 0;
				start_free = (char *)malloc_alloc::allocate(bytes_to_get);
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return (chunk_alloc(size, nobjs));
		}
	}
}
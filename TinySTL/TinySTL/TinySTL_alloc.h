#ifndef _ALLOC_
#define _ALLOC_
#include <cstdlib>

namespace TinySTL
{
	template<int inst>
	class __malloc_alloc_template
	{
	private:
		/*处理内存不足情况*/
		static void *oom_malloc(size_t);
		static void *oom_realloc(void *,size_t);
		static void(*__malloc_alloc_oom_handler)();
	
	public:
		static void * allocate(size_t n)
		{
			void *result = malloc(n);	//第一级配置器直接使用malloc()
			if (result == 0)
				result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t /*n*/)
		{
			free(p);	//第一级配置器直接使用free()
		}

		static void * reallocate(void *p, size_t /*old_sz*/, size_t new_sz)
		{
			void *result = realloc(p, new_sz);
			if (result == 0)
				result = oom_realloc(p, new_sz);
			return result;
		}

		static void(*set_malloc_handler(void(*f)())) ()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return (old);
		}

	};
	typedef __malloc_alloc_template<0> malloc_alloc;

	enum {__ALIGN = 8};
	enum {__MAX_BYTES = 128};
	enum {__NFREELISTS = __MAX_BYTES/__ALIGN};
	
	//第二级配置器
	template <bool threads, int inst>
	class __default_alloc_template
	{

	private:
		static size_t ROUND_UP(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
		}
	private:
		union obj
		{
			union obj * free_list_link;
			char client_date[1];
		};
	private:
		static obj * volatile free_list[__NFREELISTS];

		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}

		static void *refill(size_t n);
		static char *chunk_alloc(size_t size, int &nobjs);

		static char *start_free;
		static char *end_free;
		static size_t heap_size;

	public:
		static void * allocate(size_t n) 
		{
			obj * volatile * my_free_list;
			obj * result;
			if (n > (size_t)__MAX_BYTES)
			{
				return (malloc_alloc::allocate(n));
			}

			my_free_list = free_list + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0)
			{
				void *r = refill(ROUND_UP(n));
				return r;
			}

			*my_free_list = result->free_list_link;
			return (result);
		}
		static void deallocate(void *p, size_t n) 
		{
			obj *q = (obj *)p;
			obj * volatile * my_free_list;
			if (n > (size_t)__MAX_BYTES)
			{
				malloc_alloc::deallocate(p, n);
				return;
			}
			
			my_free_list = free_list + FREELIST_INDEX(n);

			q->free_list_link = *my_free_list;
			*my_free_list = q;
			return;
		}
		static void * reallocate(void *p, size_t old_sz, size_t new_sz);
	};


	/*
	template<class T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		set_new_handler(0);
		T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (tmp == 0)
		{
			cerr << "Out of memory" << endl;
			exit(1);
		}
		return tmp;
	}

	template<class T>
	inline void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	template<class T1, class T2>
	inline void _construct(T1* p, const T2& value)
	{
		new(p) T1(value);
	}

	template<class T>
	inline void _destroy(T* ptr)
	{
		ptr->~T();
	}

	template<class T>
	class allocator
	{
	public:
		typedef T			value_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

		template<class U>
		struct rebind
		{
			typedef allocator<U> other;
		};

		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate((difference_type)n, (pointer)0);
		}

		void deallocate(pointer p, size_type n)
		{
			_deallocate(p);
		}

		void construct(pointer p, const T& value)
		{
			_construct(p, value);
		}

		void destroy(pointer p)
		{
			_destroy(p);
		}

		pointer address(reference x)
		{
			return (pointer)&x;
		}

		const_pointer const_address(const_reference x)
		{
			return (const_pointer)&x;
		}

		size_type max_size() const
		{
			return size_type(UINT_MAX / sizeof(T));
		}
	};*/
}
#endif // !_ALLOC_


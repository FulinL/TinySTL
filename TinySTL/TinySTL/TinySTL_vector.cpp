#include "stdafx.h"
#include <algorithm>
#include "TinySTL_vector.h"

namespace TinySTL
{
	template<class T, class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const T& x)
	{
		if (finish != end_of_storage)		//还有备用空间
		{
			construct(finish, *(finish - 1));
			finish++;
			std::copy_backward(position, finish - 2, finish - 1);
			*position = x;
			return;
		}
		else	//没有备用空间
		{
			const size_type old_size = size();
			const size_type len = (old_size != 0 ? 2 * old_size : 1);
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = uninitialized_copy(start, position, new_start);

			construct(new_finish, x);
			new_finish++;
			new_finish = uninitialized_copy(position, finish, new_finish);

			destroy(begin(), end());
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;

		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
	{
		if (n != 0)
		{
			if (size_type(end_of_storage - finish) >= n)
			{
				T x_copy = x;

				const size_type elems_after = finish - position;
				iterator old_finish = finish;
				if (elems_after > n)//插入点之后现有元素大于新增元素个数
				{
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					std::copy_backward(position, old_finish - n, old_finish);
					fill(position, position + n, x_copy);
				}
				else
				{
					uninitialized_fill_n(finish, n - elems_after, x_copy);
					finish += n - elems_after;
					uninitialized_copy(position, old_finish, finish);
					finish += elems_after;
					fill(position, old_finish, x_copy);
				}
			}
			else
			{
				const size_type old_size = size();
				const size_type len = old_size + max(old_size, n);
				iterator new_start = data_allocator::allocate(len);
				iterator new_finish = new_start;

				new_finish = uninitialized_copy(start, position, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(position, finish, new_finish);

				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + len;
			}
		}
	}

}
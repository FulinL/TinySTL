#ifndef __LIST_
#define __LIST_

#include <algorithm>
#include "TinySTL_alloc.h"
#include "TinySTL_uninitialized.h"
#include "TinySTL_construct.h"

namespace TinySTL
{
	template<class T>
	struct __list_node
	{
		typedef void* void_pointer;
		void_pointer prev;
		void_pointer next;
		T data;
	};

	template<class T, class Ref, class Ptr>
	struct __list_iterator
	{
	public:
		typedef __list_iterator<T, T&, T*>		iterator;
		typedef __list_iterator<T, Ref, Ptr>	self;

		typedef bidirectional_iterator_tag		iterator_category;
		typedef TinySTL							value_type;
		typedef Ptr								pointer;
		typedef Ref								reference;
		typedef __list_node<T>*					link_type;
		typedef	size_t							size_type;
		typedef	ptrdiff_t						difference_type;

		link_type node;

		__list_iterator(link_type x) :node(x) {}
		__list_iterator() {}
		__list_iterator(const iterator& x) :node(x.node) {}

		bool operator == (const self& x)const { return node == x.node; }
		bool operator != (const self& x)const { return node != x.node; }

		reference operator *() const { return (*node).data; }
		poointer operator-> () const { return &(operator*()); }

		self& operator ++()
		{
			node = (link_type)((*node).next);
			return *this;
		}
		self operator ++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}

		self& operator --()
		{
			node = (link_type)((*node).prev);
			return *this;
		}
		self operator --(int)
		{
			self tmp = *this;
			--*this;
			return tmp;
		}

	};

	template<class T, class Alloc = alloc>
	class list
	{
	protected:
	 	typedef __list_node<T> list_node;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;
	protected:
		typedef __list_node<t> list_node;
	public:
		typedef list_node* link_type;
	protected:
		link_type node;

	public:
		iterator begin() { return (link_type)((*node).next); }
		iterator end() { return node; }
		bool empty() { return node->next == node; }
		size_type size()const
		{
			return (size_type)(distance(begin(), end()));
		}
		reference front() { return *begin(); }
		reference back() { return *(--end()); }

	protected:
		link_type get_node() { return list_node_allocator::allocate(); }
		void put_node(link_type p) { list_node_allocator::deallocate(p); }

		link_type creat_node(const T& x)
		{
			link_type p = get_node();
			construct(&p->data, x);
			return p;
		}
		void destroy_node(link_type p)
		{
			destroy(&p->data);
			put_node(p);
		}
	protected:
		void empty_initialize()
		{
			node = get_node();
			node->next = node;
			node->prev = node;
		}

	public:
		list() { empty_initialize(); }
		iterator insert(iterator position, const T& x)
		{
			link_type tmp = creat_node(x);
			tmp->next = position->next;
			tmp->prev = position->prev;
			(link_type(position.node->prev))->next = tmp;
			position.node->prev = tmp;
			return tmp;
		}
		void push_back(const T& x) { insert(end(), x); }
		void push_front(const T& x) { insert(begin(), x); }

		iterator erase(iterator position)
		{
			link_type next_node = link_type(position.node->next);
			link_type prev_node = link_type(position.node->prev);
			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(position.node);
			return iterator(next_node);
		}
		void pop_front() { erase(begin()); }
		void pop_back() 
		{
			iterator tmp = end(); 
			erase(--tmp); 
		}
		
		void clear()
		{
			link_type cur = (link_type)node->next;
			while (cur != node)
			{
				link_type tmp = cur;
				cur = (link_type)cur->next;
				destroy_node(tmp);
			}
			node->next = node;
			node->prev = node;
		}

		void remove(const T& value)
		{
			iterator first = begin();
			iterator last = end();
			while (first != last)
			{
				iterator next = first;
				++next;
				if (*first == value)
				{
					erase(first);
				}
				first = next;
			}
		}

		void unique()
		{
			iterator first = begin();
			iterator lase = end();
			if (first == last)
				return;
			iterator next = first;
			while (++next != last)
			{
				if (*first == *next)
				{
					erase(next);
				}
				else
				{
					first = next;
				}
				next = first;
			}
		}

		protected:
			void transfer(iterator position, iterator first, iterator last)
			{
				if (position != first)
				{
					(*(link_type((*last.node).prev))).next = position.node;
					(*(link_type((*first.node).prev))).next = last.node;
					(*(link_type((*position.node).prev))).next = first.node;
					link_type tmp = link_type((*position.node).prev);
					(*position.node).prve = (*last.node).prev;
					(*last.node).prev = (*first.node).prev;
					(*first.node).prev = tmp;
				}
			}

		public:
			void splice(iterator position, list& x)
			{
				if (!x.empty())
				{
					transfer(position, x.begin(), x.end());
				}
			}

			void splice(iterator position, list &, iterator i)
			{
				iterator j = i;
				++j;
				if (position == i || position == j)
					return;
				transfer(position, i, j);
			}

			void splice(iterator position, list &, iterator first, iterator last)
			{
				if (first != last)
					transfer(position, first, last);
			}

			void merge(list& x)	//将x合并到*this上 两个list必须先递增排序
			{
				iterator first1 = begin();
				iterator first2 = x.begin();
				iterator last1 = end();
				iterator last2 = x.end();

				while (first1 != last1 && first2 != last2)
				{
					if (*first2 < *first1)
					{
						iterator next = first2;
						transfer(first1, first2, ++next);
						first2 = next;
					}
					else
					{
						++first1;
					}
					if (first2 != last2)
					{
						transfer(last1, first2, last2);
					}
				}
			}

			void reverse()	//内容逆向重置
			{
				if (node->next == node || link_type(node->next)->next == node)
				{
					return;
				}

				iterator first = begin();
				++first;
				while (first != end())
				{
					iterator old = first;
					++first;
					transfer(begin(), old, first);
				}
			}
	};
}

#endif // !__LIST_

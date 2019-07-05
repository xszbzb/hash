#pragma once
//Ë«ÏòÑ­»·Á´±í

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MY_NEW(p, type_size) {\
p = new type_size;\
if(!p){printf("MY_NEW failed\r\n");exit(1);}}

template<class T>
class CList {
private:
	struct node {
		node *next, *prev;
		T data;
	};

	node * next;
	node * prev;
	size_t _size;

public:
	typedef int (*pFuncSort)(T& node1, T& node2);
	typedef size_t size_type;

	class iterator
	{
		node *_ptr;
	public:
		iterator(node *ptr = NULL)
		{
			_ptr = ptr;
		}

		bool operator==(const iterator &it)
		{
			return _ptr == it._ptr;
		}
		
		node * operator->()
		{
			return _ptr;
		}

		bool operator!=(const iterator &it)
		{
			return _ptr != it._ptr;
		}

		iterator operator++()
		{
			_ptr = _ptr->next;
			return _ptr;
		}

		iterator operator--()
		{
			_ptr = _ptr->prev;
			return _ptr;
		}

		iterator operator++(int)
		{
			iterator p = _ptr;
			_ptr = _ptr->next;
			return p;
		}

		iterator operator--(int)
		{
			iterator p = _ptr;
			_ptr = _ptr->prev;
			return p;
		}

		T& operator*()
		{
			return _ptr->data;
		}

		operator node *()
		{
			return _ptr;
		}
	};

public:
	
	CList()
	{
		next = (node*)this;
		prev = (node*)this;
		_size = 0;
	}

	CList(const CList &other)
	{
		next = (node*)this;
		prev = (node*)this;
		_size = 0;
		*this += other;
	}

	~CList()
	{
		clear();
	}

	CList& operator+=(const CList &other)
	{
		for (node* it = other.next; it != (node*)&other; it = it->next)
		{
			push_back(it->data);
		}
		return *this;
	}

	CList& operator=(const CList &other)
	{
		clear();
		*this += other;
		return *this;
	}

	size_type size()
	{
		return _size;
	}

	bool empty()
	{
		return next == this;
	}

	iterator begin()
	{
		return next;
	}

	iterator end()
	{
		return (node*)this;
	}

	iterator rbegin()
	{
		return prev;
	}

	iterator rend()
	{
		return (node*)this;
	}

	T& front()
	{
		return next->data;
	}

	node& back()
	{
		return prev->data;
	}

	void insert(iterator pos, const T &data)
	{
		node* new_list;
		MY_NEW(new_list, node);
		new_list->data = data;
		new_list->prev = pos->prev;
		new_list->next = pos;
		new_list->prev->next = new_list;
		pos->prev = new_list;
		_size++;
	}

	void push_front(const T &data)
	{
		insert(next, data);
	}

	void push_back(const T &data)
	{
		insert((node*)this, data);
	}

	iterator erase(iterator pos)
	{
		iterator repos = pos->prev->next = pos->next;
		pos->next->prev = pos->prev;
		delete pos;
		_size--;
		return repos;
	}

	iterator erase(iterator start, iterator end)
	{
		iterator repos = end->next;
		while (repos != (start = erase(start)));
		return repos;
	}

	void pop_front()
	{
		erase(next);
	}

	void pop_back()
	{
		erase(prev);
	}

	void clear()
	{
		while ((node*)this != next)
			erase(next);
	}

	void swap(iterator node1, iterator node2)
	{
		if (node1 != node2 && node1->next->next != node1)
		{
			if (node1->next == node2)
			{
				node1->next = node2->next;
				node2->prev = node1->prev;
				node2->next = node1;
				node1->prev = node2;
				node1->next->prev = node1;
				node2->prev->next = node2;
			}
			else if (node2->next == node1)
			{
				node2->next = node1->next;
				node1->prev = node2->prev;
				node1->next = node2;
				node2->prev = node1;
				node2->next->prev = node2;
				node1->prev->next = node1;
			}
			else
			{
				node1->prev->next = node1->next;
				node2->prev->next = node1->prev;
				node1->prev = node2->prev;
				node1->next = node2->next;
				node2->prev = node1->prev->next;
				node2->next = node2->prev->next;
				node1->next->prev = node1;
				node1->prev->next = node1;
				node2->next->prev = node2;
				node2->prev->next = node2;
			}
		}
	}

	static int cmp(T& node1, T& node2)
	{
		return memcmp(&node1, &node2, sizeof(T));
	}

	void sort(pFuncSort pFunc = cmp)
	{
		for (node* tail = (node*)this; tail != next; tail = tail->prev)
		{
			for (node* pos = next; pos->next != tail; )
			{
				if (pFunc(pos->data, pos->next->data) > 0)
				{
					swap(pos, pos->next);
				}
				else
				{
					pos = pos->next;
				}
			}
		}
	}
};

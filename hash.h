#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define RECORDER_MAX_BYTE_COUNT 0X100

template<class T>
class CHash
{
public:

	CHash(unsigned int list_size_limit = 0XFFFFFFFF)
	{
		_size = 0;
		_list_size_limit = list_size_limit;
		memset(_head, 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
	}

	virtual ~CHash()
	{
		clear();
	}
	
	struct CKey
	{
		unsigned int _key;
		CKey(unsigned int key)
		{
			_key = key;
		}

		CKey(const char* key)
		{
			_key = CHash::GetKyeForStr(key);
		}

		operator unsigned int()
		{
			return _key;
		}
	};

	virtual void clear()
	{
		for (int i = 0; i < RECORDER_MAX_BYTE_COUNT; i++)
		{
			if (_head[i])
			{
				for (int j = 0; j < RECORDER_MAX_BYTE_COUNT; j++)
				{
					if (_head[i][j])
					{
						for (int k = 0; k < RECORDER_MAX_BYTE_COUNT; k++)
						{
							if (_head[i][j][k])
							{
								for (int l = 0; l < RECORDER_MAX_BYTE_COUNT; l++)
								{
									if (_head[i][j][k][l])
									{
										delete _head[i][j][k][l];
										_size--;
									}
								}
								delete[]_head[i][j][k];
							}
						}
						delete[]_head[i][j];
					}
				}
				delete[]_head[i];
				_head[i] = NULL;
			}
		}
	}

	//功能: 判断对应的索引中是否有相同的对象已经记录
	//参数: key是索引,最大支持4字节,为了确保不会使用太多内存,注意选择值的大小范围，越小占用内存越小
	//		other是要记录和查询是否有同样记录存在的对象的引用
	//返回: 如果没有查到同样的记录,返回NULL; 如果查到了返回对象的指针
	virtual T* find(const CKey key, const T &other)
	{
		const unsigned char *k = (const unsigned char *)&key;
		if (!_head[k[3]])
		{
			return NULL;
		}
		if (!_head[k[3]][k[2]])
		{
			return NULL;
		}
		if (!_head[k[3]][k[2]][k[1]])
		{
			return NULL;
		}
		if (!_head[k[3]][k[2]][k[1]][k[0]])
		{
			return NULL;
		}
		return find_for_list(*_head[k[3]][k[2]][k[1]][k[0]], other);
	}

	virtual void push_back(const CKey key, const T &other)
	{
		const unsigned char *k = (const unsigned char *)&key;
		if (!_head[k[3]])
		{
			MY_NEW(_head[k[3]], CList<T>***[RECORDER_MAX_BYTE_COUNT]);
			memset(_head[k[3]], 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
		}
		if (!_head[k[3]][k[2]])
		{
			MY_NEW(_head[k[3]][k[2]], CList<T>**[RECORDER_MAX_BYTE_COUNT]);
			memset(_head[k[3]][k[2]], 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
		}
		if (!_head[k[3]][k[2]][k[1]])
		{
			MY_NEW(_head[k[3]][k[2]][k[1]], CList<T>*[RECORDER_MAX_BYTE_COUNT]);
			memset(_head[k[3]][k[2]][k[1]], 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
		}
		if (!_head[k[3]][k[2]][k[1]][k[0]])
		{
			MY_NEW(_head[k[3]][k[2]][k[1]][k[0]], CList<T>);
		}
		push_back(*_head[k[3]][k[2]][k[1]][k[0]], other);
	}

	virtual T* find_add(const CKey key, const T& other)
	{
		const unsigned char* k = (const unsigned char*)& key;
		if (!_head[k[3]])
		{
			MY_NEW(_head[k[3]], CList<T> * **[RECORDER_MAX_BYTE_COUNT]);
			memset(_head[k[3]], 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
		}
		if (!_head[k[3]][k[2]])
		{
			MY_NEW(_head[k[3]][k[2]], CList<T> * *[RECORDER_MAX_BYTE_COUNT]);
			memset(_head[k[3]][k[2]], 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
		}
		if (!_head[k[3]][k[2]][k[1]])
		{
			MY_NEW(_head[k[3]][k[2]][k[1]], CList<T> * [RECORDER_MAX_BYTE_COUNT]);
			memset(_head[k[3]][k[2]][k[1]], 0, RECORDER_MAX_BYTE_COUNT * sizeof(void*));
		}
		if (!_head[k[3]][k[2]][k[1]][k[0]])
		{
			MY_NEW(_head[k[3]][k[2]][k[1]][k[0]], CList<T>);
		}
		T* p = find(key, other);
		if(NULL == p)
			push_back(*_head[k[3]][k[2]][k[1]][k[0]], other);
		return p;
	}

	static unsigned int GetKeyForStr(const char* str)
	{
		unsigned int uRet = 0;
		char *pDst = (char*)&uRet;
		char *pSrc = (char*)&uRet;
		int len = sizeof(uRet);
		while (*str)
		{
			*pDst++ ^= *str++;
			if (pDst - pSrc == len)
				pDst = pSrc;
		}
		return uRet;
	}

private:

	virtual T* find_for_list(CList<T> &list_t, const T &other)
	{
		typename CList<T>::iterator it = list_t.begin();
		for (; it != list_t.end(); it++)
		{
			if (*it == other)
				return &*it;
		}
		return NULL;
	}

	virtual void push_back(CList<T> &list_t, const T &other)
	{
		list_t.push_back(other);
		if (_list_size_limit < list_t.size())
			list_t.pop_front();
		else
			_size++;
	}

	CList<T> ****_head[RECORDER_MAX_BYTE_COUNT];
	size_t _size;
	size_t _list_size_limit;
};

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "hash.h"
#include <string>
using namespace std;

struct Test
{
	int a;
	int b;
	bool operator==(const Test &other)
	{
		return memcmp(&other, this, sizeof(other)) == 0;
	}
};

int main()
{
	int count = 0;
	int times = 30;
	Test t = {0};
	char buf[100];

	//test 1
	CHash<Test> r(28);
	unsigned int key = 1;
	for (key = 1; key < 0xfffffff; key <<= 1)
	{
		t.a = key;
		for (int size_list = 0; size_list < times; size_list++)
		{
			count++;
			t.a++;
			if (r.find(key, t))
			{
				printf("error 1 key=%d\r\n", key);
			}
			else
			{
				r.push_back(key, t);
			}
		}
	}
	
	for (key = 1; key < 0xfffffff; key <<= 1)
	{
		t.a = key;
		for (int size_list = 0; size_list < times; size_list++)
		{
			count++;
			t.a++;
			if (!r.find(key, t))
			{
				printf("error 1 key=%d\r\n", key);
			}
		}
	}

	//test 2
	CHash<string> rStr(29);
	for (key = 1; key < 0xfffffff; key <<= 1)
	{
		t.a = key;
		for (int size_list = 0; size_list < times; size_list++)
		{
			count++;
			t.a++;
			sprintf(buf, "str %d", t.a);
			string str(buf);
			rStr.push_back(key, str);
			if (!rStr.find(key, str))
			{
				printf("error 2 str=%s\r\n", str.c_str());
			}
		}
	}

	for (key = 1; key < 0xfffffff; key <<= 1)
	{
		t.a = key;
		for (int size_list = 0; size_list < times; size_list++)
		{
			count++;
			t.a++;
			sprintf(buf, "str %d", t.a);
			string str(buf);
			if (!rStr.find(key, str))
			{
				printf("error 2 str=%s\r\n", str.c_str());
			}
		}
	}
	rStr.clear();

	//test 3
	CHash<string> rStr3;
	for (key = 1; key < 0xfffffff; key <<= 1)
	{
		t.a = key;
		for (int size_list = 0; size_list < times; size_list++)
		{
			count++;
			t.a++;
			sprintf(buf, "str %d", t.a);
			string str(buf);
			rStr3.push_back(key, str);
			if (!rStr3.find(key, str))
			{
				printf("error 3 str=%s\r\n", str.c_str());
			}
		}
	}

	for (key = 1; key < 0xfffffff; key <<= 1)
	{
		t.a = key;
		for (int size_list = 0; size_list < times; size_list++)
		{
			count++;
			t.a++;
			sprintf(buf, "str %d", t.a);
			string str(buf);
			if (!rStr3.find(key, str))
			{
				printf("error 3 str=%s\r\n", str.c_str());
			}
		}
	}

	return 1;
}


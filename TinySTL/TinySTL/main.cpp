// TinySTL.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "TinySTL_alloc.h"
#include "TinySTL_alloc.cpp"
#include "TinySTL_iterator.h"
#include "TinySTL_type_traits.h"
#include "TinySTL_construct.h"
#include "TinySTL_vector.h"
#include "TinySTL_vector.cpp"
#include "TinySTL_uninitialized.h"

using namespace std;

int main()
{
	//vector test
	//TinySTL::vector<int>::iterator ivi;
	TinySTL::vector<int> iv(2,9);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(1);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;

	iv.push_back(2);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(3);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(4);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(5);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(6);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(7);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;
	iv.push_back(8);
	cout << "size = " << iv.size() << endl;
	cout << "capacity = " << iv.capacity() << endl;
	cout << endl;
	for (auto ix : iv)
	{
		cout << ix << endl;
	}
	cout << endl;

	system("pause");
    return 0;
}


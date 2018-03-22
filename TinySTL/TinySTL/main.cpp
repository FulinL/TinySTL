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
	iv.push_back(1);
	//cout << "size = " << iv.size() << endl;
	//cout << "capacity = " << iv.capacity() << endl;
	system("pause");
    return 0;
}


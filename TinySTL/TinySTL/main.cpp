// TinySTL.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TintSTL_alloc.h"

using namespace std;

int main()
{
	TinySTL::allocator<int> ialloc;
	int *ptr = ialloc.allocate(2);
	ialloc.construct(ptr, 10);
	cout << *ptr << endl;
	system("pause");
    return 0;
}


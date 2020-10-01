#include <stdio.h>
#include "Alg.h"
#include "AlgCPP.h"

// C语言风格的接口调用例子
int main1()
{
	void *a;
	initAlg(&a);
	setRadius(a, 2);
	printf("面积 = %f\n", getArea(a));
	unInitAlg(a);
	return 1;
}

// C++风格的接口调用例子
int main()
{
	AlgCPP a;
	a.setRadius(2);
	printf("面积 = %f\n", a.getArea());
}
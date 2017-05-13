#include <iostream>
#include <time.h>
#include "opencv2/opencv.hpp"

void test()
{
	int a = 0;
	for (int i = 0; i<100000000; i++)
		a++;
	//printf("a = %d\n", a);
}

int main1()
{
	clock_t t1 = clock();
	double t = (double)cv::getTickCount();
	for (int i = 0; i<8; i++)
		test();
	clock_t t2 = clock();
	std::cout << "time: " << t2 - t1 << std::endl;
	printf("%f\n", ((double)cv::getTickCount() - t) / cv::getTickFrequency());
	return 0;
}

// openmp
int main2()
{
	clock_t t1 = clock();
#pragma omp parallel for
	for (int i = 0; i<8; i++)
		test();
	clock_t t2 = clock();
	std::cout << "time: " << t2 - t1 << std::endl;
	return 0;
}
// fork, join
// openmp
int main3()
{
	#pragma omp parallel for
	for (int i = 0; i < 10; i++)
		std::cout << i << std::endl;

	std::cout << "hello" << std::endl;
	return 0;
}

// openmp
int main()
{
	int sum = 0;
	int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	#pragma omp parallel for
	for (int i = 0; i < 100000; i++)
	{
		#pragma omp critical
		{
			sum = sum + a[i % 10];
		}
	}
	std::cout << "sum: " << sum << std::endl;
	return 0;
}


//int main(){
//	int a[20] = { 11, 2, 33, 49, 113, 20, 321, 250, 689, 16, 
//		11, 2, 33, 49, 113, 20, 321, 250, 690, 16 };
//	int max = a[0];
//	
//#pragma omp parallel for
//	for (int i = 1; i<20; i++)
//	{
//		
//		//#pragma omp critical
//		{
//			if (a[i] > max)
//				max = a[i];
//		}
//	}
//	std::cout << "max: " << max << std::endl;
//	return 0;
//}
#pragma once
#include <vector>
class Circle
{
public:
	Circle();
	~Circle();

	int setRadius(int r);
	int getRadius();
	float getArea();

private:
	int radius;
	std::vector<int> v;
};



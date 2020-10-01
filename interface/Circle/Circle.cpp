#include "Circle.h"
Circle::Circle()
{
}
Circle::~Circle()
{

}

int Circle::setRadius(int r)
{
	radius = r;
	return 1;
}
int Circle::getRadius()
{
	return radius;
}
float Circle::getArea()
{
	return 3.14 * radius * radius;
}

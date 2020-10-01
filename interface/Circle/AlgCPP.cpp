
#include "AlgCPP.h"
#include "Circle.h"

AlgCPP::AlgCPP()
{
	c = new Circle();
}
AlgCPP::~AlgCPP()
{
	delete c;
}

int AlgCPP::setRadius(int r)
{
	((Circle *)c)->setRadius(r);
	return 1;
}

float AlgCPP::getArea()
{
	return ((Circle *)c)->getArea();
}
#include "Alg.h"
#include "Circle.h"
typedef struct
{
	Circle circle;

} Alg;

void initAlg(void **h)
{

	Alg *c = new Alg();
	*h = c;
}
void unInitAlg(void *h)
{
	if (h)
		delete h;
}

int setRadius(void *h, int r)
{
	Alg *c = (Alg *)h;
	c->circle.setRadius(r);
	return 1;
}


float getArea(void *h)
{
	Alg *c = (Alg *)h;
	return c->circle.getArea();
}

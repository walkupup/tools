#pragma once
#include <vector>

#ifdef DLLProvider
#define DLL_EXPORT_IMPORT __declspec(dllexport)
#else
#define DLL_EXPORT_IMPORT __declspec(dllimport)
#endif

class DLL_EXPORT_IMPORT AlgCPP
{
public:
	AlgCPP();
	~AlgCPP();
	int setRadius(int r);
	float getArea();

private:
	void *c;

};



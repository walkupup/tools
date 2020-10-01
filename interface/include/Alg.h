#pragma once

#ifdef DLLProvider
#define DLL_EXPORT_IMPORT __declspec(dllexport)
#else
#define DLL_EXPORT_IMPORT __declspec(dllimport)
#endif

DLL_EXPORT_IMPORT void initAlg(void **h);
DLL_EXPORT_IMPORT int setRadius(void *h, int r);
DLL_EXPORT_IMPORT float getArea(void *h);
DLL_EXPORT_IMPORT void unInitAlg(void *h);
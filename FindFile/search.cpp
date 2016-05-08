#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;
bool Search(char *Path, char *File, std::vector<string> &vec);
bool main()
{
	std::vector<string> vec;
	vec.clear();
	Search("blacklist", "*.*", vec);
	return true;
}

bool Search(char *Path, char *File, std::vector<string> &vec)
{
	HANDLE hFind;
	WIN32_FIND_DATA wfd;
	ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
	char PathTemp[512];
	memset(PathTemp, 0, sizeof(PathTemp));
	sprintf(PathTemp, "%s\\*.*", Path);
	hFind = FindFirstFile(PathTemp, &wfd);
	if (INVALID_HANDLE_VALUE == hFind)//如果没有找到或者查找失败
	{
		MessageBox(NULL, "INVALID_HANDLE_VALUE", "FindFirstFile", MB_OK);
		return false;
	}
	do
	{
		if ('.' == wfd.cFileName[0])//如果是当前目录或者是上级目录，就直接进入下一次循环
		{
			continue;
		}
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//如果该文件是文件夹
		{
			sprintf(PathTemp, "%s\\%s", Path, wfd.cFileName);
			//MessageBox(NULL, PathTemp, "Directory", MB_OK);
			printf("%s\n", PathTemp);
			Search(PathTemp, File, vec);//进入该文件夹继续搜索(递归)
		}
		else
		{
			//if (0 == strcmp(File, wfd.cFileName))
			{
				//MessageBox(NULL, "Found", wfd.cFileName, MB_OK);//找到文件
				sprintf(PathTemp, "%s\\%s", Path, wfd.cFileName);
				printf("%s\n", PathTemp);
				vec.push_back(PathTemp);
			}
		}
	} while (FindNextFile(hFind, &wfd));//文件查找的条件
	FindClose(hFind);//关闭FIND句柄
	return true;
}
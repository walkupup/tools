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
	if (INVALID_HANDLE_VALUE == hFind)//���û���ҵ����߲���ʧ��
	{
		MessageBox(NULL, "INVALID_HANDLE_VALUE", "FindFirstFile", MB_OK);
		return false;
	}
	do
	{
		if ('.' == wfd.cFileName[0])//����ǵ�ǰĿ¼�������ϼ�Ŀ¼����ֱ�ӽ�����һ��ѭ��
		{
			continue;
		}
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//������ļ����ļ���
		{
			sprintf(PathTemp, "%s\\%s", Path, wfd.cFileName);
			//MessageBox(NULL, PathTemp, "Directory", MB_OK);
			printf("%s\n", PathTemp);
			Search(PathTemp, File, vec);//������ļ��м�������(�ݹ�)
		}
		else
		{
			//if (0 == strcmp(File, wfd.cFileName))
			{
				//MessageBox(NULL, "Found", wfd.cFileName, MB_OK);//�ҵ��ļ�
				sprintf(PathTemp, "%s\\%s", Path, wfd.cFileName);
				printf("%s\n", PathTemp);
				vec.push_back(PathTemp);
			}
		}
	} while (FindNextFile(hFind, &wfd));//�ļ����ҵ�����
	FindClose(hFind);//�ر�FIND���
	return true;
}
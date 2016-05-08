#include "stdlib.h"
#include "direct.h"
#include "string.h"
#include "string"
#include "io.h"
#include "stdio.h" 
#include <vector>
#include <iostream>
#include <fstream>
#include <io.h>

#include "Windows.h"
using namespace std;

#define _findfirst _findfirst32i64
#define _findnext _findnext32i64
#define _finddata_t _finddata32i64_t



class CBrowseDir
{
protected:
	//��ų�ʼĿ¼�ľ���·������'\'��β
	char m_szInitDir[_MAX_PATH];

public:
	//ȱʡ������
	CBrowseDir();

	//���ó�ʼĿ¼Ϊdir���������false����ʾĿ¼������
	bool SetInitDir(const char *dir);

	//��ʼ������ʼĿ¼������Ŀ¼����filespecָ�����͵��ļ�
	//filespec����ʹ��ͨ��� * ?�����ܰ���·����
	//�������false����ʾ�������̱��û���ֹ
	bool BeginBrowse(const char *filespec);
	vector<string> BeginBrowseFilenames(const char *filespec);

protected:
	//����Ŀ¼dir����filespecָ�����ļ�
	//������Ŀ¼,���õ����ķ���
	//�������false,��ʾ��ֹ�����ļ�
	bool BrowseDir(const char *dir, const char *filespec);
	vector<string> GetDirFilenames(const char *dir, const char *filespec);
	//����BrowseDirÿ�ҵ�һ���ļ�,�͵���ProcessFile
	//�����ļ�����Ϊ�������ݹ�ȥ
	//�������false,��ʾ��ֹ�����ļ�
	//�û����Ը�д�ú���,�����Լ��Ĵ������
	virtual bool ProcessFile(const char *filename);

	//����BrowseDirÿ����һ��Ŀ¼,�͵���ProcessDir
	//�������ڴ����Ŀ¼������һ��Ŀ¼����Ϊ�������ݹ�ȥ
	//������ڴ�����ǳ�ʼĿ¼,��parentdir=NULL
	//�û����Ը�д�ú���,�����Լ��Ĵ������
	//�����û�����������ͳ����Ŀ¼�ĸ���
	virtual void ProcessDir(const char *currentdir, const char *parentdir);
};

CBrowseDir::CBrowseDir()
{
	//�õ�ǰĿ¼��ʼ��m_szInitDir
	_getcwd(m_szInitDir, _MAX_PATH);

	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
		strcat(m_szInitDir, "\\");
}

bool CBrowseDir::SetInitDir(const char *dir)
{
	//�Ȱ�dirת��Ϊ����·��
	if (_fullpath(m_szInitDir, dir, _MAX_PATH) == NULL)
		return false;

	//�ж�Ŀ¼�Ƿ����
	if (_chdir(m_szInitDir) != 0)
		return false;

	//���Ŀ¼�����һ����ĸ����'\',����������һ��'\'
	int len = strlen(m_szInitDir);
	if (m_szInitDir[len - 1] != '\\')
		strcat(m_szInitDir, "\\");

	return true;
}

vector<string> CBrowseDir::BeginBrowseFilenames(const char *filespec)
{
	ProcessDir(m_szInitDir, NULL);
	return GetDirFilenames(m_szInitDir, filespec);
}

bool CBrowseDir::BeginBrowse(const char *filespec)
{
	ProcessDir(m_szInitDir, NULL);
	return BrowseDir(m_szInitDir, filespec);
}

bool CBrowseDir::BrowseDir(const char *dir, const char *filespec)
{
	_chdir(dir);

	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(filespec, &fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�������,����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy(filename, dir);
				strcat(filename, fileinfo.name);
				cout << filename << endl;
				if (!ProcessFile(filename))
					return false;
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	//����dir�е���Ŀ¼
	//��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���
	//��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��
	//ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼
	//��_findnextû��Ӱ�졣
	_chdir(dir);
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�����,�ټ���ǲ��� . �� .. 
			//�������,���е���
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp
					(fileinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy(subdir, dir);
					strcat(subdir, fileinfo.name);
					strcat(subdir, "\\");
					ProcessDir(subdir, dir);
					if (!BrowseDir(subdir, filespec))
						return false;
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return true;
}

vector<string> CBrowseDir::GetDirFilenames(const char *dir, const char *filespec)
{
	_chdir(dir);
	vector<string>filename_vector;
	filename_vector.clear();

	//���Ȳ���dir�з���Ҫ����ļ�
	long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(filespec, &fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�������,����д���
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				char filename[_MAX_PATH];
				strcpy(filename, dir);
				strcat(filename, fileinfo.name);
				filename_vector.push_back(filename);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	//����dir�е���Ŀ¼
	//��Ϊ�ڴ���dir�е��ļ�ʱ���������ProcessFile�п��ܸı���
	//��ǰĿ¼����˻�Ҫ�������õ�ǰĿ¼Ϊdir��
	//ִ�й�_findfirst�󣬿���ϵͳ��¼���������Ϣ����˸ı�Ŀ¼
	//��_findnextû��Ӱ�졣
	_chdir(dir);
	if ((hFile = _findfirst("*.*", &fileinfo)) != -1)
	{
		do
		{
			//����ǲ���Ŀ¼
			//�����,�ټ���ǲ��� . �� .. 
			//�������,���е���
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp
					(fileinfo.name, "..") != 0)
				{
					char subdir[_MAX_PATH];
					strcpy(subdir, dir);
					strcat(subdir, fileinfo.name);
					strcat(subdir, "\\");
					ProcessDir(subdir, dir);
					vector<string>tmp = GetDirFilenames(subdir, filespec);
					for (vector<string>::iterator it = tmp.begin(); it < tmp.end(); it++)
					{
						filename_vector.push_back(*it);
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return filename_vector;
}

bool CBrowseDir::ProcessFile(const char *filename)
{
	return true;
}

void CBrowseDir::ProcessDir(const char
	*currentdir, const char *parentdir)
{
}

//��CBrowseDir�����������࣬����ͳ��Ŀ¼�е��ļ�����Ŀ¼����
class CStatDir :public CBrowseDir
{
protected:
	int m_nFileCount;   //�����ļ�����
	int m_nSubdirCount; //������Ŀ¼����

public:
	//ȱʡ������
	CStatDir()
	{
		//��ʼ�����ݳ�Աm_nFileCount��m_nSubdirCount
		m_nFileCount = m_nSubdirCount = 0;
	}

	//�����ļ�����
	int GetFileCount()
	{
		return m_nFileCount;
	}

	//������Ŀ¼����
	int GetSubdirCount()
	{
		//��Ϊ�����ʼĿ¼ʱ��Ҳ����ú���ProcessDir��
		//���Լ�1�������������Ŀ¼������
		return m_nSubdirCount - 1;
	}

protected:
	//��д�麯��ProcessFile��ÿ����һ�Σ��ļ�������1
	virtual bool ProcessFile(const char *filename)
	{
		m_nFileCount++;
		return CBrowseDir::ProcessFile(filename);
	}

	//��д�麯��ProcessDir��ÿ����һ�Σ���Ŀ¼������1
	virtual void ProcessDir
		(const char *currentdir, const char *parentdir)
	{
		m_nSubdirCount++;
		CBrowseDir::ProcessDir(currentdir, parentdir);
	}
};

void main1()
{
	//��ȡĿ¼��
	char buf[256];
	printf("������Ҫͳ�Ƶ�Ŀ¼��:");
	gets(buf);

	//���������
	CStatDir statdir;

	//����Ҫ������Ŀ¼
	if (!statdir.SetInitDir(buf))
	{
		puts("Ŀ¼�����ڡ�");
		return;
	}

	//��ʼ����

	vector<string>file_vec = statdir.BeginBrowseFilenames("*.*");
	for (vector<string>::const_iterator it = file_vec.begin(); it < file_vec.end(); ++it)
		std::cout << *it << std::endl;

	printf("�ļ�����: %d\n", file_vec.size());
	system("pause");
}


int FilePath_GetAllFile(string name, vector<string> &fileVec)
{
	char pBuf[200];
	GetCurrentDirectory(200, pBuf);
	CStatDir statdir;
	if (!statdir.SetInitDir(name.c_str()))
	{
		puts("Ŀ¼�����ڡ�");
		return -1;
	}

	//��ʼ����
	fileVec = statdir.BeginBrowseFilenames("*.*");

	SetCurrentDirectory(pBuf);
	return 1;
}

string FilePath_GetExt(string name)
{
	int pos = name.rfind(".");
	if (pos == string::npos)
		return string();
	return name.substr(pos + 1);
}
string FilePath_GetFileName(string name)
{
	int pos = name.rfind("/");
	int start, end;
	if (pos == string::npos)
	{
		pos = name.rfind("\\");
		if (pos == string::npos)
			return string();
		else
			start = pos + 1;
	}
	else
	{
		start = pos + 1;
	}

	pos = name.rfind(".");
	if (pos == string::npos)
		end = name.length() - 1;
	else
		end = pos - 1;

	return name.substr(start, end - start + 1);
}
string FilePath_GetFileParent(string name)
{
	int pos = name.rfind("/");
	string div;
	int start, end;
	if (pos == string::npos)
		div = "/";
	else
		div = "\\";

	end = name.rfind(div) - 1;
	start = name.rfind(div, end) + 1;

	return name.substr(start, end - start + 1);
}

//string FilePath_GetFileParent2()
//{
//
//}

int fileListFromTxt(string fileName, vector<string> &vec)
{
	FILE *fp;
	int lines = 0;
	string name;
	//if ((fp = fopen(fileName.c_str(), "rt")) == NULL)
	//{
	//	printf("file list not exsit\n");
	//	return lines;
	//}
	std::ifstream infile(fileName);

	while (infile >> name)
	{
		vec.push_back(name);
	}
	return vec.size();
}

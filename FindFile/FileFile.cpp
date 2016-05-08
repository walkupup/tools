
#include "afx.h"
#include <string>
#include <vector>
using namespace std;

void TraverseFile(CString strDir, CString strExt)
{
	CFileFind findFile;

	CString strWildcard = strDir + _T("\\*.*");

	BOOL fSuccess = findFile.FindFile(strWildcard);
	while (fSuccess)
	{
		fSuccess = findFile.FindNextFile();

		if (findFile.IsDots()) {
			continue;
		}

		if (findFile.IsDirectory())
		{
			CString strDirRecurse = strDir + _T("\\") + findFile.GetFileName();
			TraverseFile(strDirRecurse, strExt);
		}
		else
		{
			CString strFilePath = findFile.GetFilePath();

			int nLen = strFilePath.GetLength();
			int nDot = strFilePath.ReverseFind('.');
			if (strFilePath.Right(nLen - nDot) == strExt)
			{
				_tprintf(_T("%s\n"), strFilePath);
			}
		}
	}

	findFile.Close();
}

// 递归遍历任何类型的文件
void TraverseFileAll(CString strDir, CString strExt)
{
	CFileFind findFile;

	CString strWildcard = strDir + _T("\\*.*");

	BOOL fSuccess = findFile.FindFile(strWildcard);
	while (fSuccess)
	{
		fSuccess = findFile.FindNextFile();

		if (findFile.IsDots()) {
			continue;
		}

		if (findFile.IsDirectory())
		{
			CString strDirRecurse = strDir + _T("\\") + findFile.GetFileName();
			TraverseFile(strDirRecurse, strExt);
		}
		else
		{
			CString strFilePath = findFile.GetFilePath();

			int nLen = strFilePath.GetLength();
			int nDot = strFilePath.ReverseFind('.');
			//if (strFilePath.Right(nLen - nDot) == strExt)
			{
				_tprintf(_T("%s\n"), strFilePath);
			}
		}
	}

	findFile.Close();
}


int _tmain1(int argc, _TCHAR* argv[])
{
	TraverseFile(_T("G:\\data\\lfw\\lfw"), _T(".*"));

	return 0;
}

#include <string>
#include <vector>
using namespace std;


int FilePath_GetAllFile(string name, vector<string> &fileVec);

int main11()
{
	//设置要遍历的目录
	vector<string> fileVec;
	if (FilePath_GetAllFile("blacklist", fileVec) < 0)
		return 0;

}
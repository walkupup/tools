#include <string>
#include <vector>
using namespace std;


int FilePath_GetAllFile(string name, vector<string> &fileVec);

int main11()
{
	//����Ҫ������Ŀ¼
	vector<string> fileVec;
	if (FilePath_GetAllFile("blacklist", fileVec) < 0)
		return 0;

}
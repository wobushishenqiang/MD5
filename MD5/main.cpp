#include<iostream>
#include"MD5.h"
using std::cout;
using std::endl;
int main(int argc, char *argv[])
{
	MD5 test;
	string a = "";
	for (int i = 0; i < 100; ++i)
	{
		a += "a";
	}
	cout << test.encode(a) << endl;

	
	system("pause");
	
	return 0;
}
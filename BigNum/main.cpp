#include<iostream>
#include"bigint.h"
using namespace std;

int main(int argc, char* argv[])
{
	BigInt a("0xF000000FF");
	a.printData();
	system("pause");
	return 0;
}
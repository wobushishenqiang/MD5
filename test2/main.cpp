#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
//gcd(a,b) = gcd(b,a mod b)
int gcd(int a, int b)
{
	
	while (b != 0)
	{
		int tmp = b;
		b = a % b;
		a = tmp;
	}
	return a;
}

int gcdEx(int a, int b, int &x, int &y)
{
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	else
	{
		int gcdNum = gcdEx(b, a%b, x, y);
		int tmp = x;
		x = y;
		y = tmp - a / b * y;
		return gcdNum;
	}


}
class constA {
public:
	std::string text = "abc";
	const int a = 1;
	const char& operator[](std::size_t position) const
	{
		cout << "const" << endl;
		return text[position];
	}

	char& operator[](std::size_t position)
	{
		cout << "nonconst" << endl;
		return text[position];
	}

	void test()
	{
		cout << a <<endl;
		cout << "nonconst" << endl;
	}
};
int main(int argc, char *argv[])
{
	vector<int> a = { 9,8,7,6,5,4,3,2,1 };
	vector<int>::iterator lit = a.begin();
	vector<int>::iterator rit = a.begin() + 4;
	cout << *lit << " " << *(rit-1) << endl;
	sort(lit, rit);
	cout << *lit << " " << *(rit-1) << endl;
	for (auto i : a)
	{
		cout << i <<" ";
	}
	cout << endl;

	
	
	system("pause");
	return 0;
}
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
using namespace std;

#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))//右移的时候，高位一定要补零，而不是补充符号位
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476
//strBaye的长度
unsigned int strlength;
//A,B,C,D的临时变量
unsigned int atemp;
unsigned int btemp;
unsigned int ctemp;
unsigned int dtemp;
//常量ti unsigned int(abs(sin(i+1))*(2pow32))
const unsigned int k[] = {
		0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
		0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
		0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
		0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
		0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
		0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
		0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
		0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
		0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
		0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
		0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
		0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
		0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };
//向左位移数
const unsigned int s[] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,
		12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
		4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
		15,21,6,10,15,21,6,10,15,21,6,10,15,21 };
const char str16[] = "0123456789abcdef";
void mainLoop(unsigned int M[])
{
	unsigned int f, g;
	unsigned int a = atemp;
	unsigned int b = btemp;
	unsigned int c = ctemp;
	unsigned int d = dtemp;
	for (unsigned int i = 0; i < 64; i++)
	{
		if (i < 16) {
			f = F(b, c, d);
			g = i;
		}
		else if (i < 32)
		{
			f = G(b, c, d);
			g = (5 * i + 1) % 16;
		}
		else if (i < 48) {
			f = H(b, c, d);
			g = (3 * i + 5) % 16;
		}
		else {
			f = I(b, c, d);
			g = (7 * i) % 16;
		}
		unsigned int tmp = d;
		d = c;
		c = b;
		b = b + shift((a + f + k[i] + M[g]), s[i]);
		a = tmp;
	}
	atemp = a + atemp;
	btemp = b + btemp;
	ctemp = c + ctemp;
	dtemp = d + dtemp;
}
/*
*填充函数
*处理后应满足bits≡448(mod512),字节就是bytes≡56（mode64)
*填充方式为先加一个1,其它位补零
*最后加上64位的原来长度
*/
unsigned int* add(string str)
{
	unsigned int num = ((str.length() + 8) / 64) + 1;//以512位,64个字节为一组
	unsigned int *strByte = new unsigned int[num * 16];    //64/4=16,所以有16个整数
	strlength = num * 16;
	for (unsigned int i = 0; i < num * 16; i++)
		strByte[i] = 0;
	for (unsigned int i = 0; i < str.length(); i++)
	{
		strByte[i >> 2] |= (str[i]) << ((i % 4) * 8);//一个整数存储四个字节，i>>2表示i/4 一个unsigned int对应4个字节，保存4个字符信息
	}
	strByte[str.length() >> 2] |= 0x80 << (((str.length() % 4)) * 8);//尾部添加1 一个unsigned int保存4个字符信息,所以用128左移
	/*
	*添加原长度，长度指位的长度，所以要乘8，然后是小端序，所以放在倒数第二个,这里长度只用了32位
	*/
	strByte[num * 16 - 2] = str.length() * 8;
	return strByte;
}
string changeHex(int a)
{
	int b;
	string str1;
	string str = "";
	for (int i = 0; i < 4; i++)
	{
		str1 = "";
		b = ((a >> i * 8) % (1 << 8)) & 0xff;   //逆序处理每个字节
		for (int j = 0; j < 2; j++)
		{
			str1.insert(0, 1, str16[b % 16]);
			b = b / 16;
		}
		str += str1;
	}
	return str;
}
string getMD5(string source)
{
	atemp = A;    //初始化
	btemp = B;
	ctemp = C;
	dtemp = D;
	unsigned int *strByte = add(source);
	for (unsigned int i = 0; i < strlength / 16; i++)
	{
		unsigned int num[16];
		for (unsigned int j = 0; j < 16; j++)
			num[j] = strByte[i * 16 + j];
		mainLoop(num);
	}
	return changeHex(atemp).append(changeHex(btemp)).append(changeHex(ctemp)).append(changeHex(dtemp));
}


const unsigned int T[] = {
	0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
	0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
	0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
	0x6b901122,0xfd987193,0xa679438e,0x49b40821,
	0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
	0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
	0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
	0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
	0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
	0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
	0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
	0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
	0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
	0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
	0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
	0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391 };


class MD5 {
private:
	unsigned int tempA, tempB, tempC, tempD, strlength;
public:
	MD5() {
		tempA = A;
		tempB = B;
		tempC = C;
		tempD = D;
		strlength = 0;
	}

	// 编码函数
	string encode(string src) {
		vector<unsigned int> rec = padding(src);
		for (unsigned int i = 0; i < strlength / 16; i++) {
			unsigned int num[16];
			for (int j = 0; j < 16; j++) {
				num[j] = rec[i * 16 + j];
			}
			iterateFunc(num, 16);
		}
		return format(tempA) + format(tempB) + format(tempC) + format(tempD);
	}
	// 循环压缩
	void iterateFunc(unsigned int* X, int size = 16) {
		unsigned int a = tempA,
			b = tempB,
			c = tempC,
			d = tempD,
			rec = 0,
			g, k;
		for (int i = 0; i < 64; i++) {
			if (i < 16) {
				// F迭代
				g = F(b, c, d);
				k = i;
			}
			else if (i < 32) {
				// G迭代
				g = G(b, c, d);
				k = (1 + 5 * i) % 16;
			}
			else if (i < 48) {
				// H迭代
				g = H(b, c, d);
				k = (5 + 3 * i) % 16;
			}
			else {
				// I迭代
				g = I(b, c, d);
				k = (7 * i) % 16;
			}
			rec = d;
			d = c;
			c = b;
			b = b + shift(a + g + X[k] + T[i], s[i]);
			a = rec;
		}
		tempA += a;
		tempB += b;
		tempC += c;
		tempD += d;
	}
	// 填充字符串
	vector<unsigned int> padding(string src) {
		// 以512位,64个字节为一组
		unsigned int remain = (src.length() * 8) % 512 == 448 ? 512 : 448 - (src.length() * 8) % 512;
		unsigned int num = ((remain + src.length() * 8) + 64) / 512;
		vector<unsigned int> rec(num * 16);
		strlength = num * 16;
		for (unsigned int i = 0; i < src.length(); i++) {
			// 一个unsigned int对应4个字节，保存4个字符信息
			rec[i >> 2] |= (int)(src[i]) << ((i % 4) * 8);
		}
		// 补充1000...000
		rec[src.length() >> 2] |= 0x80 << ((src.length() % 4) * 8);
		// 填充原文长度
		rec[rec.size() - 2] = src.length() << 3;
		return rec;
	}
	// 整理输出
	string format(unsigned int num) {
		string res = "";
		unsigned int base = 1 << 8;
		for (int i = 0; i < 4; i++) {
			string tmp = "";
			unsigned int b = (num >> (i * 8)) % base & 0xff;
			for (int j = 0; j < 2; j++) {
				tmp = str16[b % 16] + tmp;
				b /= 16;
			}
			res += tmp;
		}
		return res;
	}
};

int main() {
	MD5 test;
	string a = "";
	 
	
	//100a 36a92cc94a9e0fa21f625f8bfb007adf
	//488a 6119b0cb0a63d23a1a5c086d4cf4c6cf
	//489 b71bb258bcebfbc888ab7582639ca378
	//512 56907396339ca2b099bd12245f936ddc
	//513 6649c3e827e44f7bf539768bddf76435
	//UINT32_MAX+1 
 	cout <<test.encode(a) << endl;


	string s = getMD5(a);
	cout << s << endl;

	system("pause");
	return 0;

}
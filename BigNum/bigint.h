#pragma once
#include<stdint.h>
#include<vector>
#include<string>

class BigInt
{
public:
	typedef uint64_t int64_t;
	typedef uint32_t int32_t;

	typedef uint32_t base_t;
	typedef int64_t  long_t;
	
	BigInt() : isnegative(false) { data.push_back(0); }// 默认为0
	BigInt(const long long &);
	BigInt(const std::string &);//1 判断十进制数、十六进制数
	
	BigInt(const BigInt &);//会被return BigInt a = b;隐式调用，所以不能声明为explicit

	~BigInt() {}

	BigInt add(const BigInt &);//加
	BigInt sub(const BigInt &);//减
	BigInt mul(const BigInt &);//乘
	BigInt div(const BigInt &);//除
	BigInt rem(const BigInt &);//取余
	BigInt mod(const BigInt &);//取模
	BigInt pow(const BigInt &);//阶乘

	BigInt shiftLeft(const unsigned);//左移
	BigInt shiftRight(const unsigned);//右移

	int compare(const BigInt &) const;//比较 -1表示本大整数要小,0表示相等,1表示本大整数要大
	bool equals(const BigInt &) const;//相等 

	std::string toHexString() const;//转换为字符串
	BigInt abs() const;//绝对值
protected:
	friend BigInt operator + (const BigInt &, const BigInt &);
	friend BigInt operator - (const BigInt &, const BigInt &);
	friend BigInt operator * (const BigInt &, const BigInt &);
	friend BigInt operator / (const BigInt &, const BigInt &);
	friend BigInt operator % (const BigInt &, const BigInt &);
	friend bool operator < (const BigInt &, const BigInt &);
	friend bool operator > (const BigInt &, const BigInt &);
	friend bool operator == (const BigInt &, const BigInt &);
	friend bool operator <= (const BigInt &, const BigInt &);
	friend bool operator >= (const BigInt &, const BigInt &);
	friend bool operator != (const BigInt &, const BigInt &);

	friend std::ostream & operator << (std::ostream &, const BigInt &);

	//隐式赋值
	//显式赋值
	BigInt operator = (const std::string & str) { return (*this) = BigInt(str); }
	BigInt operator = (const long_t & num) { return (*this) = BigInt(num); }
private:
	void clear();

	void trim();
	
	bool isHex(char);
	base_t hexToNum(char);//十六进制转十进制
	char numToHex(base_t) const;

	base_t charToNum(char);
	
	base_t base_zero = 0;
	base_t base_max = UINT32_MAX;
	int base_bits = sizeof(base_t) << 3;
	// X进制转大数需要几个X进制数
	
	int hex_base = base_bits / 4;//十六进制为4个二进制
private:
	std::vector<base_t> data;//主体以32位4字节储存
	bool isnegative = false;
	uintmax_t len;

public:
	void printData();
};
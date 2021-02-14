#include"bigint.h"
#include<assert.h>
#include<iostream>
BigInt::BigInt(const std::string &num)
{
	//判断字符串是十进制、十六进制
	if (num.size() == 0)
		return;

	enum numType {Oct, Num, Hex} numtype;

	//判断正负
	int start = 0;

	if (num[0] == '-')
	{
		isnegative = true;
		++start;
	}

	//十进制
	if (num.size() == 1)
		numtype = Num;
	else
	{
		numtype = Num;
		if (num[start] == '0')
		{
			numtype = Oct;
			if (num[start + 1] == 'X' || num[start + 1] == 'x')
			{
				numtype = Hex;
			}
		}
	}

	int length = num.length();
	switch (numtype)
	{
	//八进制
	case Oct:
	{
		
	}break;
		
	case Num:
	{
		
	}break;

	case Hex:
	{
		//每1个char为1个十六进制位4个二进制位。一个32位数需要8个char填进来
		for (int i = num.length() - 1; i >= start + 2 ; i -= hex_base)
		{
			base_t temp = 0;
			base_t carry = 0;
			for (int j = 0; j < hex_base; ++j)
			{
				if (i - j < start + 2)
					break;
				
				assert(isHex(num[i - j]));
				temp += (hexToNum(num[i - j] ) << carry);
				
				carry += 4;
			}
			data.push_back(temp);
		}
	}break;
	default:
		bool wrongString = true;
		assert(wrongString);
		break;
	}
	
}

BigInt::BigInt(const BigInt &rightval)
{
	//调用隐式赋值
	*this = rightval;
}

BigInt::BigInt(const long long &val)
{
	while (0 != val)
	{
		data.push_back(val & base_max);
		val >> 32;
	}
}

BigInt BigInt::add(const BigInt &other)
{
	BigInt res(*this);
	if (this->isnegative == other.isnegative)
	{
		int len = other.data.size() - res.data.size();

		while (len-- > 0)
			res.data.push_back(0);

		
		int carry = 0;
		for (int i = 0; i < other.data.size(); ++i)
		{
			base_t temp = res.data[i];
			res.data[i] += other.data[i] + carry;
			carry = (res.data[i] < temp ? 1 : (temp > temp + other.data[i] ? 1 : 0));
			// a + b + c = x
			// x < a + b + c 1

			// x > a + b + c 0
			// x == a + b + c
			// a > a + b 1 
			// a <= a + b 0
		}

		for (int i = other.data.size(); i < res.data.size() && carry != 0; ++i)
		{
			base_t temp = res.data[i];
			res.data[i] += carry;
			carry = temp > res.data[i] ? 1 : 0;
		}

		if (carry != 0)
		{
			res.data.push_back(carry);
		}
	}
	else
	{
		BigInt a = abs();
		BigInt b = other.abs();
		int flag = a.compare(b);
		if (flag == 0)
		{
			return BigInt(0);
		}
		else if (flag < 0)
		{
			res = b.sub(a);
			res.isnegative = b.isnegative;
		}
		else 
		{
			res = a.sub(b);
			res.isnegative = a.isnegative;
		}
	}
	return res;
	
}

BigInt BigInt::sub(const BigInt &other)
{
	BigInt res(*this);
	BigInt a = abs();
	BigInt b = other.abs();
	if (isnegative == other.isnegative)
	{
		int flag = compare(other);
		if (flag > 0)
		{
			base_t borrow = 0;
			for (int i = 0; i < other.data.size(); ++i)
			{
				base_t temp = res.data[i];
				res.data[i] = res.data[i] - other.data[i] - borrow;
				borrow = (res.data[i] > temp ? 1 : (temp - other.data[i] > temp? 1 : 0));
			}

			//还有借位
			for (int i = other.data.size(); i < data.size() && borrow != 0; ++i)
			{
				base_t temp = res.data[i];
				res.data[i] -= borrow;
				borrow = temp < res.data[i] ? 1 : 0;
			}
			//去除高位0
			res.trim();
		}
		else if (flag == 0)
		{
			return BigInt(0);
		}
		else
		{
			res = b.sub(a);
			res.isnegative = !isnegative;
		}
	}
	else
	{
		res = a.add(b);
		res.isnegative = isnegative;
	}
	return res;
}

BigInt BigInt::mul(const BigInt &)
{
	return BigInt();
}

BigInt BigInt::div(const BigInt &)
{
	return BigInt();
}

BigInt BigInt::rem(const BigInt &)
{
	return BigInt();
}

BigInt BigInt::mod(const BigInt &)
{
	return BigInt();
}

BigInt BigInt::pow(const BigInt &)
{
	return BigInt();
}

BigInt BigInt::shiftLeft(const unsigned)
{
	return BigInt();
}

BigInt BigInt::shiftRight(const unsigned)
{
	return BigInt();
}

int BigInt::compare(const BigInt &other) const
{
	if (isnegative != other.isnegative)
	{

	}
	int flag = 0;
	if (data.size() < other.data.size())
	{
		return -1;
	}
	else if (data.size() > other.data.size())
	{
		return  1;
	}
	for (int i = 0; i < data.size(); ++i)
	{

	}
	return 0;
}

bool BigInt::equals(const BigInt &other) const
{
	return (isnegative == other.isnegative) && (data == other.data);
}

//大数转字符串
std::string BigInt::toHexString()  const
{
	std::string res = "0x";
	base_t highfour = base_max << ( base_bits - 4 );
	for (int i = data.size() - 1; i >= 0; ++i)
	{
		base_t temp = data[i];
		for (int j = 0; j < hex_base; j++)
		{
			res += numToHex((temp & highfour) >> (base_bits));
			temp <<= 4;
		}
		
	}
	return res;
}

//返回绝对值
BigInt BigInt::abs() const
{
	BigInt res(*this);//复制构造
	res.isnegative = false;//非负
	return res;
}

BigInt operator + (const BigInt &a, const BigInt & b)
{
	BigInt t(a);
	return t.add(b);
}

BigInt operator - (const BigInt &a, const BigInt & b)
{
	BigInt t(a);
	return t.sub(b);
}

BigInt operator * (const BigInt &a, const BigInt &b)
{
	BigInt t(a);
	return t.mul(b);
}

BigInt operator / (const BigInt &a, const BigInt &b)
{
	BigInt t(a);
	return t.div(b);
}

BigInt operator % (const BigInt &a, const BigInt &b)
{
	BigInt t(a);
	return t.mod(b);
}

bool operator < (const BigInt &a, const BigInt &b)
{
	return a.compare(b) == -1;
}

bool operator > (const BigInt &a, const BigInt &b)
{
	return b < a;
}

bool operator == (const BigInt &a, const BigInt &b)
{
	return a.equals(b);
}

bool operator <= (const BigInt &a, const BigInt &b)
{
	return !(a > b);
}

bool operator >= (const BigInt &a, const BigInt &b)
{
	return !(a < b);
}

bool operator != (const BigInt &a, const BigInt &b)
{
	return !(a == b);
}

//流输出
std::ostream & operator << (std::ostream &out, const BigInt &val)
{
	//转为字符串
	out << val.toHexString();
	return out;
}

void BigInt::trim()
{
	int count = 0;
	for(std::vector<base_t>::const_reverse_iterator it = data.rbegin(); it != data.rend(); ++it)
	{
		if (*it == 0)
		{
			count++;
		}
		else
			break;
		if (count == data.size())
			return;
		for (int i = 0; i < count; ++i)
		{
			data.pop_back();
		}
	}
}

//判断字符是否在十六进制范围数内
bool BigInt::isHex(char c)
{
	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}



BigInt::base_t BigInt::hexToNum(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else
		return c - 'A' + 10;
}

char BigInt::numToHex(base_t num) const
{
	if (num < 10)
		return num + '0';
	else
		return num - 10 + 'A';
}

/*
//判断字符是否在八进制范围数内
bool BigInt::isOct(char c)
{
	return (c >= '0' && c <= '7');
}

//八进制转十进制
BigInt::base_t BigInt::octToNum(char c)
{
	return c - '0';
}
*/

BigInt::base_t BigInt::charToNum(char c)
{
	return c - '0';
}

void BigInt::printData()
{
	std::cout << "data size: " << data.size() << std::endl;
	int index = 0;
	for (auto i : data)
	{
		index++;
		std::cout <<index<<" : "<< i << std::endl;
	}
}
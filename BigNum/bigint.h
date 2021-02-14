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
	
	BigInt() : isnegative(false) { data.push_back(0); }// Ĭ��Ϊ0
	BigInt(const long long &);
	BigInt(const std::string &);//1 �ж�ʮ��������ʮ��������
	
	BigInt(const BigInt &);//�ᱻreturn BigInt a = b;��ʽ���ã����Բ�������Ϊexplicit

	~BigInt() {}

	BigInt add(const BigInt &);//��
	BigInt sub(const BigInt &);//��
	BigInt mul(const BigInt &);//��
	BigInt div(const BigInt &);//��
	BigInt rem(const BigInt &);//ȡ��
	BigInt mod(const BigInt &);//ȡģ
	BigInt pow(const BigInt &);//�׳�

	BigInt shiftLeft(const unsigned);//����
	BigInt shiftRight(const unsigned);//����

	int compare(const BigInt &) const;//�Ƚ� -1��ʾ��������ҪС,0��ʾ���,1��ʾ��������Ҫ��
	bool equals(const BigInt &) const;//��� 

	std::string toHexString() const;//ת��Ϊ�ַ���
	BigInt abs() const;//����ֵ
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

	//��ʽ��ֵ
	//��ʽ��ֵ
	BigInt operator = (const std::string & str) { return (*this) = BigInt(str); }
	BigInt operator = (const long_t & num) { return (*this) = BigInt(num); }
private:
	void clear();

	void trim();
	
	bool isHex(char);
	base_t hexToNum(char);//ʮ������תʮ����
	char numToHex(base_t) const;

	base_t charToNum(char);
	
	base_t base_zero = 0;
	base_t base_max = UINT32_MAX;
	int base_bits = sizeof(base_t) << 3;
	// X����ת������Ҫ����X������
	
	int hex_base = base_bits / 4;//ʮ������Ϊ4��������
private:
	std::vector<base_t> data;//������32λ4�ֽڴ���
	bool isnegative = false;
	uintmax_t len;

public:
	void printData();
};
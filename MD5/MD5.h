#pragma once
#include<string>
using std::string;
/*
	ժҪ�㷨MD5
*/
class MD5 {
public:
	string encode(string key);
	string file_encode(string filepath);
	void setSalt(string salt);
private:
	
	uintmax_t length;
	string key;
	string salt;
	string MD5Value = "";

	uintmax_t blockNum;
	void calMD5();
	void update();
	void init();
	string format(uint32_t num);
	
	uint32_t tmpA = 0x67452301,
		tmpB = 0xefcdab89,
		tmpC = 0x98badcfe,
		tmpD = 0x10325476;

	// F����
	inline uint32_t F(uint32_t X, uint32_t Y, uint32_t Z) {
		return (X & Y) | ((~X) & Z);
	}
	// G����
	inline uint32_t G(uint32_t X, uint32_t Y, uint32_t Z) {
		return (X & Z) | (Y & (~Z));
	}
	// H����
	inline uint32_t H(uint32_t X, uint32_t Y, uint32_t Z) {
		return X ^ Y ^ Z;
	}
	// I����
	inline uint32_t I(uint32_t X, uint32_t Y, uint32_t Z) {
		return Y ^ (X | (~Z));
	}
	// ��λ��������
	inline uint32_t shift(uint32_t X, uint32_t N) {
		return (X << N) | (X >> (32 - N));
	}
};
#pragma once
#include<string>
using std::string;
class RSA {
public:
	string encode(string str);
	string decode(string str);
private:
	uint32_t primeA;
	uint32_t primeB;

	uint64_t primeAccu;//key
	uint64_t primePhi;

	uint64_t epsilon;//public key
	uint64_t d;//private key

	void gcd();
	void gcdEx();
	void createKey();
	void init();
};
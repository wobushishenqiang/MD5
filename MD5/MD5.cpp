#include"MD5.h"

uint32_t M[16] = { 0 };

const char str16[] = "0123456789abcdef";

//����
const unsigned int randomVal[] = {
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

//�����λ��
const unsigned int randomMove[] = { 7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
									5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
									4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
									6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21 };

void MD5::init()
{
		tmpA = 0x67452301;
		tmpB = 0xefcdab89;
		tmpC = 0x98badcfe;
		tmpD = 0x10325476;
		MD5Value = "";
}

void MD5::calMD5()
{
	
	//512�����ĸ���
	blockNum = (length + 8) / 64 + 1;

	//padLen = ceil((length + 64) / 512);
	/*��Ŀ������Ϊ�޷��ţ�����ֵ�ǵ���Դֵģ 2n����С�޷���ֵ��
	���� n �����ڱ�ʾĿ�����͵�λ������ȡ����Ŀ�����͸�����խ��
	�ֱ���з��������з�����չ[��ע 1]��ضϣ������޷�����������
	��չ��ضϡ�*/
	uint32_t lenBig   = length << 3 >> 32, 
			 lenSmall = length << 3;

	uintmax_t stringIndex = 0;
	bool isFarBlock = false;
	//һ�������洢�ĸ��ֽڣ�i>>2��ʾi/4 һ��unsigned int��Ӧ4���ֽڣ�����4���ַ���Ϣ
	for (uintmax_t i = 0; i < blockNum; ++i)
	{
		
		for (int j = 0; j < 16; ++j)
		{
			//0x 0 0 0 0
			//  a  b  c  d
			//0x d c b a
			if (stringIndex == length)
			{
				break;
			}
			//0x 0 0 0 a 
			M[j] |= (key[stringIndex]) << ((stringIndex % 4) * 8);//��λ��һλ����0�ֽڲ��һ�����
			stringIndex++;
			if (stringIndex == length)
			{
				//bλ���0x80 1000 0000
				M[j] |= 0x80 << 8;
				break;
			}

			//0x 0 0 b a
			M[j] |= (key[stringIndex]) << ((stringIndex % 4) * 8);//��λ�ڶ�λ����1�ֽڲ��һ�����
			stringIndex++;
			if (stringIndex == length)
			{
				//cλ���0x80
				M[j] |= 0x80 << 16;
				break;
			}

			//0x 0 c b a
			M[j] |= (key[stringIndex]) << ((stringIndex % 4) * 8);//��λ����λ����2�ֽڲ��һ�����
			stringIndex++;
			if (stringIndex == length)
			{
				//dλ���0x80
				M[j] |= 0x80 << 24;
				break;
			}

			//0x d c b a
			M[j] |= (key[stringIndex]) << ((stringIndex % 4) * 8);//��λ����λ����3�ֽڲ��һ�����
			stringIndex++;
			if (stringIndex == length)
			{
				//aλ���0x80
				if (j + 1 == 16)
					isFarBlock = true;
				else
					M[j+1] |= 0x80 ;
				break;
			}
		}
		//������
		if (i == blockNum - 1)
		{
			M[14] = lenSmall;
			M[15] = lenBig;
			if (isFarBlock)
				M[0] |= 0x80;
		}

		//ÿ����������FGHI����
		update();
		
		for (int j = 0; j < 16; ++j)
		{
			M[j] = 0;
		}
	}
	
	MD5Value = format(tmpA) + format(tmpB) + format(tmpC) + format(tmpD);
	
}

void MD5::update()
{
	uint32_t a = tmpA,
             b = tmpB,
             c = tmpC,
             d = tmpD,
             tmp = 0,
             ret, 
			 k;
        for(int i = 0; i < 64; i++) {
            if(i < 16) {
                // F����
                ret = F(b, c, d);
                k = i;
            }
            else if(i < 32) {
                // G����
                ret = G(b, c, d);
                k = (1 + 5*i) % 16;
            }
            else if(i < 48) {
                // H����
                ret = H(b, c, d);
                k = (5 + 3*i) % 16;
            }
            else {
                // I����
                ret = I(b, c, d);
                k = (7*i) % 16;
            }
            tmp = d;
            d = c;
            c = b;
            b = b + shift(a + ret + M[k] + randomVal[i], randomMove[i]);
            a = tmp;
        }
        tmpA += a;
        tmpB += b;
        tmpC += c;
        tmpD += d;
}

string MD5::format(uint32_t num) {
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

string MD5::encode(string key)
{
	length = key.length();
	this->key = key;
	init();
	calMD5();
	return MD5Value;
}

void MD5::setSalt(string salt)
{
	this->salt = salt;
}

string MD5::file_encode(string filepath)
{

}
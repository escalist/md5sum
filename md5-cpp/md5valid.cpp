#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "md5valid.h"


typedef unsigned DigestArray[4];

unsigned func0(unsigned abcd[]) {
	std::cout << std::hex << "1 " << abcd[0] << ' ' << abcd[1] << ' ' << abcd[2] << ' ' << abcd[3] << std::endl;
	auto result = (abcd[1] & abcd[2]) | (~abcd[1] & abcd[3]);
	return result;
}

unsigned func1(unsigned abcd[]) {
	std::cout << std::hex << "2 " << abcd[0] << ' ' << abcd[1] << ' ' << abcd[2] << ' ' << abcd[3] << std::endl;
	return (abcd[3] & abcd[1]) | (~abcd[3] & abcd[2]);
}

unsigned func2(unsigned abcd[]) {
	std::cout << std::hex << "3 " << abcd[0] << ' ' << abcd[1] << ' ' << abcd[2] << ' ' << abcd[3] << std::endl;
	return  abcd[1] ^ abcd[2] ^ abcd[3];
}

unsigned func3(unsigned abcd[]) {
	std::cout << std::hex << "4 " << abcd[0] << ' ' << abcd[1] << ' ' << abcd[2] << ' ' << abcd[3] << std::endl;
	return abcd[2] ^ (abcd[1] | ~abcd[3]);
}

typedef unsigned(*DgstFctn)(unsigned a[]);

unsigned* calctable(unsigned* k)
{
	double s, pwr;
	int i;

	pwr = pow(2.0, 32);
	for (i = 0; i < 64; i++) {
		s = fabs(sin(1.0 + i));
		k[i] = (unsigned)(s * pwr);
	}
	return k;
}

unsigned rol(unsigned r, short N)
{
	unsigned  mask1 = (1 << N) - 1;
	return ((r >> (32 - N)) & mask1) | ((r << N) & ~mask1);
}

unsigned* Algorithms_Hash_MD5(const char* msg, int mlen)
{
	static DigestArray h0 = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476 };
	static DgstFctn ff[] = { &func0, &func1, &func2, &func3 };
	static short M[] = { 1, 5, 3, 7 };
	static short O[] = { 0, 1, 5, 0 };
	static short rot0[] = { 7, 12, 17, 22 };
	static short rot1[] = { 5, 9, 14, 20 };
	static short rot2[] = { 4, 11, 16, 23 };
	static short rot3[] = { 6, 10, 15, 21 };
	static short* rots[] = { rot0, rot1, rot2, rot3 };
	static unsigned kspace[64];
	static unsigned* k;

	static DigestArray h;
	DigestArray abcd;
	DgstFctn fctn;
	short m, o, g;
	unsigned f;
	short* rotn;
	union {
		unsigned w[16];
		char     b[64];
	}mm;
	int os = 0;
	int grp, grps, q, p;
	unsigned char* msg2;

	if (k == NULL) k = calctable(kspace);

	for (q = 0; q < 4; q++) {
		h[q] = h0[q];
	}

	{
		grps = 1 + (mlen + 8) / 64;
		msg2 = (unsigned char*)malloc(64 * grps);
		memcpy(msg2, msg, mlen);
		msg2[mlen] = (unsigned char)0x80;
		q = mlen + 1;
		while (q < 64 * grps) { msg2[q] = 0; q++; }
		{
			MD5union u;
			u.w = 8 * mlen;
			q -= 8;
			memcpy(msg2 + q, &u.w, 4);
		}
	}

	for (grp = 0; grp < grps; grp++) {
		memcpy(mm.b, msg2 + os, 64);
		for (int i = 0; i < 16; ++i) {			
			std::cout << std::hex << mm.w[i] << std::endl;
		}
		for (q = 0; q < 4; q++) abcd[q] = h[q];
		for (p = 0; p < 4; p++) {
			fctn = ff[p];
			rotn = rots[p];
			m = M[p]; o = O[p];
			for (q = 0; q < 16; q++) {
				g = (m * q + o) % 16;
				auto fcntval = fctn(abcd);
				//std::cout << std::hex << "THIS - " << abcd[0] << ' ' << fcntval << ' ' << k[q + 16 * p] << ' ' << mm.w[g] << ' ' << g << std::endl;
				auto value = abcd[0] + fcntval + k[q + 16 * p] + mm.w[g];
				f = abcd[1] + rol(value, rotn[q % 4]);
				//std::cout << std::hex << "TEST - " << value << ' ' << rotn[q % 4] << ' ' << rol(value, rotn[q % 4]) << std::endl;

				abcd[0] = abcd[3];
				abcd[3] = abcd[2];
				abcd[2] = abcd[1];
				abcd[1] = f;
				//std::cout << std::hex << abcd[0] << ' ' << abcd[1] << ' ' << abcd[2] << ' ' << abcd[3] << std::endl;
			}

		}
		
		for (p = 0; p < 4; p++) {
			h[p] += abcd[p];
		}
			
		os += 64;		
	}

	return h;
}

void GetMD5String(const char* msg, int mlen) {
	char str[33];
	strcpy(str, "");
	int j, k;
	unsigned* d = Algorithms_Hash_MD5(msg, strlen(msg));

	MD5union u;
	for (j = 0; j < 4; j++) {
		u.w = d[j];
		printf("%02x%02x%02x%02x", u.b[0], u.b[1], u.b[2], u.b[3]);
	}
	return;
}
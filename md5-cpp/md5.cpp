#include "md5.h"
#include "md5valid.h"

#include <iostream>

namespace chvlal {

	unsigned int concat(char a, char b, char c, char d) {
		unsigned int result = 0xFFFFFFFFFFFFFFFF;
		unsigned int ai = (unsigned int)a;
		unsigned int bi = (unsigned int)b;
		unsigned int ci = (unsigned int)c;
		unsigned int di = (unsigned int)d;

		unsigned int ds = di << 24 & 0xFF000000;
		unsigned int cs = ci << 16 & 0x00FF0000;
		unsigned int bs = bi << 8 & 0x0000FF00;
		unsigned int as = ai & 0x000000FF;

		result = as | bs | cs | ds;
		return result;
	}

	int calculateMaxLength(int length) {
		const int chunksize = 64;
		int d = ( length + 8) / chunksize;
		int o = ( length + 8) % chunksize;
		if (0 == o) {
			return d * chunksize;
		}
		return (d + 1) * chunksize;
	}

	char getNextByte(const char* input, int length, int index) {
		int realIndex = index;
		if (realIndex < length) {
			return input[realIndex];
		}

		if (realIndex == length) {
			return 0x80;
		}

		int maxlength = calculateMaxLength(length);
		
		if (index < maxlength - 8) {
			return 0;
		}
		if (index >= maxlength - 4) {			
			return 0;
		}
		
		long int bytenum = maxlength - index - 4;
		
		unsigned long value = 8 * length;

		union {
			unsigned w;
			unsigned char b[4];
		} lunion;

		lunion.w = value;		

		char result = lunion.b[4 - bytenum];

		return result;
	}


	unsigned leftrotate(unsigned r, short N)
	{
		unsigned  mask1 = (1 << N) - 1;
		return ((r >> (32 - N)) & mask1) | ((r << N) & ~mask1);
	}

	md5s getMd5Value(const char* input, int length)
	{
		// s specifies the per-round shift amounts
		int s[64] = { 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
						5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
						4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
						6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };

		// (Or just use the following precomputed table):
		unsigned int K[64] = { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
								0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
								0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
								0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
								0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
								0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
								0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
								0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
								0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
								0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
								0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
								0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
								0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
								0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
								0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
								0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

		// Initialize variables:
		int a0 = 0x67452301;   // A
		int b0 = 0xefcdab89;   // B
		int c0 = 0x98badcfe;   // C
		int d0 = 0x10325476;   // D

		// Pre-processing: adding a single 1 bit
		int maxlen = calculateMaxLength(length);
		int totalChunks = maxlen / 64; 


		for (int ai = 0; ai < totalChunks; ++ai) {
			unsigned int M[16];
			for (int j = 0; j < 16; ++j) {
				char values[4];
				for (int k = 0; k < 4; ++k) {
					int offset = ai * 64 + 4 * j + k;					
					values[k] = getNextByte(input, length, offset);
				}
				M[j] = concat(values[0], values[1], values[2], values[3]);
			}

			for (int i = 0; i < 16; ++i) {
				std::cout << std::hex << M[i] << std::endl;
			}

			// Initialize hash value for this chunk:
			unsigned int A = a0;
			unsigned int B = b0;
			unsigned int C = c0;
			unsigned int D = d0;
			// Main loop:
			for (int i = 0; i < 64; ++i) {
				unsigned int F, g;
				if (0 <= i && i <= 15) {
					std::cout << std::hex << "1 " << A << ' ' << B << ' ' << C << ' ' << D << std::endl;
					F = (B & C) | (~B & D);
					g = i;
				}
				else if (16 <= i && i <= 31) {
					std::cout << std::hex << "2 " << A << ' ' << B << ' ' << C << ' ' << D << std::endl;
					F = (D & B) | ((~D) & C);
					g = (5 * i + 1) % 16;
				}
				else if (32 <= i && i <= 47) {
					std::cout << std::hex << "3 " << A << ' ' << B << ' ' << C << ' ' << D << std::endl;
					F = B ^ C ^ D;
					g = (3 * i + 5) % 16;
				}
				else if (48 <= i && i <= 63) {
					std::cout << std::hex << "4 " << A << ' ' << B << ' ' << C << ' ' << D << std::endl;
					F = C ^ (B | (~D));
					g = (7 * i) % 16;
				}
				// Be wary of the below definitions of a,b,c,d				
				//std::cout << std::hex << "THIS - " << A << ' ' << F << ' ' << K[i] << ' ' << M[g] << ' ' << g << std::endl;
				F = F + A + K[i] + M[g];  // M[g] must be a 32-bits block	
				A = D;
				D = C;
				C = B;
				//std::cout << std::hex << "TEST - " << F << ' ' << s[i] << ' ' << leftrotate(F, s[i]) << std::endl;
				B = B + leftrotate(F, s[i]);
				//std::cout << std::hex << A << ' ' << B << ' ' << C << ' ' << D << std::endl;
			}
			// Add this chunk's hash to result so far:
			a0 = a0 + A;
			b0 = b0 + B;
			c0 = c0 + C;
			d0 = d0 + D;
		}

		md5s result;
		result.v[0] = a0;
		result.v[1] = b0;
		result.v[2] = c0;
		result.v[3] = d0;

		return result;
	}

}
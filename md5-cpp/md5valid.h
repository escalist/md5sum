#ifndef CHVLAL_MD5_VALID
#define CHVLAL_MD5_VALID

typedef union uwb {
	unsigned w;
	unsigned char b[4];
} MD5union;

void GetMD5String(const char* msg, int mlen);

#endif
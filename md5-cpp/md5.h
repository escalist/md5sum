#ifndef CHVLAL_MD5_H
#define CHVLAL_MD5_H

namespace chvlal {

	typedef struct {
		unsigned int v[4];
	} md5s;

	md5s getMd5Value(const char* input, int length);

}

#endif
#include <iostream>

#include "md5.h"
#include "md5valid.h"

int main()
{
	//const char* input = "The quick brown fox jumps over the lazy dog.";
	//const char* md5expected = "e4d909c290d0fb1ca068ffaddf22cbd0";
	const char* input = "1";
	//const char* md5expected = "d41d8cd98f00b204e9800998ecf8427e";
	int len = strlen(input);

	int j;
	std::cout << "Invalid" << std::endl;
	chvlal::md5s result = chvlal::getMd5Value(input, len);
	{
		MD5union u;
		for (j = 0; j < 4; j++) {
			u.w = result.v[j];
			printf("%02x%02x%02x%02x", u.b[0], u.b[1], u.b[2], u.b[3]);			
		}
		std::cout << std::endl;
	}
		
	std::cout << std::endl;	
	std::cout << "valid" << std::endl;
	std::cout << std::endl;

	GetMD5String(input, len);


	return 0;
}
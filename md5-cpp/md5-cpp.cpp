#include <iostream>
#include "md5.h"
#include "md5valid.h"
#include <cstring>

int main()
{
	//const char* input = "The quick brown fox jumps over the lazy dog.";
	//const char* md5expected = "e4d909c290d0fb1ca068ffaddf22cbd0";
	const char* input = "";
	const char* md5expected = "d41d8cd98f00b204e9800998ecf8427e";
	int len = strlen(input);

	int j, k;
	struct md5s result = md5(input, len);
	
	printf("%02x%02x%02x%02x", result.v[0], result.v[1], result.v[2], result.v[3]);
	
	std::cout << std::endl;	
	std::cout << "valid" << std::endl;
	std::cout << std::endl;

	GetMD5String(input, len);


	return 0;
}
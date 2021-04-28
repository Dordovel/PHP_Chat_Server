//
// Created by dordovel on 28.04.2021.
//

#include <iostream>
#include "header/Ssl.h"

int main()
{
	Ssl ssl;
	auto keys = ssl.ssl_keys();
	std::string encode = ssl.ssl_encode("Test", keys["PUBLIC"]);
	std::cout<<"Encode: "<<encode<<std::endl;

	std::string decode = ssl.ssl_decode(encode, keys["PRIVATE"]);
	std::cout<<"Decode: "<<decode<<std::endl;
	return EXIT_SUCCESS;
}

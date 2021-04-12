#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <libgen.h>
#include <array>


std::string get_path()
{
	std::string path = "";

	std::array<char, 2048> buffer;

	if(readlink("/proc/self/exe", buffer.data(), buffer.size()) > 0)
		path = dirname(buffer.data());

	return path;
}

enum class Type
{
	ENCODE,
	DECODE
};

std::string ssl(const std::string& str, Type type)
{
	std::string path = get_path();

	const std::string exec = "php";
	const std::string file = "decode.php";

	std::string params;

	if(type == Type::DECODE)
		params = "--decode";
	else if(type == Type::ENCODE)
		params = "--encode";


	const std::string decoder = exec +
							" " + path + "/" +
							file + " " + params +
							" " + str;

	char buffer[128];

	std::string data;

	FILE* pipe = popen(decoder.c_str(), "r");

	if(!pipe) throw std::runtime_error("pipe not open");

	try
	{
		while(fgets(buffer, sizeof buffer, pipe) != NULL)
		{
			data += std::string(buffer);
		}
	}
	catch(std::exception &ex)
	{
		std::cout<<ex.what()<<std::endl;
		pclose(pipe);
		throw;
	}

	pclose(pipe);

	return data;
}

int main()
{

	std::cout<<ssl("jsdlfjasdf", Type::DECODE)<<std::endl;

	return EXIT_SUCCESS;
}

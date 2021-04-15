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
	DECODE,
	PRIVATE,
	PUBLIC
};

std::string get_query_type(Type type)
{
	std::string params = "";

	switch(type)
	{
		case Type::DECODE:
			params = "--decode";
		break;
		case Type::ENCODE:
			params = "--encode";
		break;
		case Type::PRIVATE:
			params = "--generate private";
		break;
		case Type::PUBLIC:
			params = "--generate public";
		break;
		default: break;
	}

	return params;
}

std::string create_query(std::string exec, std::string file, Type type, const std::string& query)
{
	std::string params = get_query_type(type);

	std::string path = get_path();

	std::string q = exec + " " + path + "/" +
							file + " " + params +
							" " + query;

	return q;
}

std::string ssl(Type type, std::string str = "")
{
	const std::string query = create_query("php", "decode.php", type, str);

	char buffer[128];

	std::string data;

	FILE* pipe = popen(query.c_str(), "r");

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

	std::cout<<ssl(Type::PRIVATE)<<std::endl;

	return EXIT_SUCCESS;
}

#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <libgen.h>
#include <array>
#include <tuple>

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

std::string get_value(const std::string& value)
{
	return value;
}

template<typename std::size_t I = 0,
		typename ...Argv>
typename std::enable_if<I == sizeof...(Argv), std::string>::type
unpack(std::tuple<Argv...> argv, const std::string& separator = " ")
{
	return "";
}

template<typename std::size_t I = 0,
        typename ...Argv>
typename std::enable_if<I < sizeof...(Argv), std::string>::type
unpack(std::tuple<Argv...> argv, const std::string& separator = " ")
{
	std::string res = std::get<I>(argv);
	if(I < (sizeof...(Argv) - 1)) res += separator;
	res += get_value(unpack<I+1>(argv));

	return res;
}

template<typename ...T>
std::string create_query_param(T&&... argv)
{
	return unpack(std::make_tuple(argv...));
}

template<typename ...T>
std::string create_query(std::string exec, std::string file, Type type, T&&... argv)
{
	std::string params = get_query_type(type);

	std::string path = get_path();

	std::string q = exec + " " + path + "/" +
							file + " " + params +
							" " + create_query_param(std::forward<T>(argv)...);

	return q;
}

std::string ssl(Type type,
				const std::string& key = "",
				const std::string& str = "")
{
	const std::string query = create_query("php", "decode.php", type, key, str);

	char buffer[128];

	std::string data;

	FILE* pipe = popen(query.c_str(), "r");

	if(!pipe) throw std::runtime_error("pipe not open");

	try
	{
		while(fgets(buffer, sizeof buffer, pipe))
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
	std::string privateKey = ssl(Type::PRIVATE);
	std::string publicKey = ssl(Type::PUBLIC);

	std::cout<<"SSL Key\t"<<privateKey<<std::endl;

	std::string msg = "Message";

	std::string encode = ssl(Type::ENCODE, publicKey, msg);

	std::cout<<"Encode\t"<<encode<<std::endl;

	return EXIT_SUCCESS;
}

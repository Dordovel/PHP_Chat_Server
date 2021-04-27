#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <libgen.h>
#include <array>
#include <tuple>
#include <fstream>
#include <cstring>
#include <unordered_map>

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
	GENERATE,
	KEY,
	PIPE
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
		case Type::GENERATE:
			params = "--generate";
		break;
		case Type::KEY:
			params = "--key";
			break;
		case Type::PIPE:
			params = "--pipe";
			break;
		default: break;
	}

	return params;
}

enum class Pipe
{
	READ,
	WRITE
};

std::string get_value(const std::string& value)
{
	return value;
}

std::string get_value(Type type)
{
	return get_query_type(type);
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
	std::string res = get_value(std::get<I>(argv));
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
std::string query(std::string exec, std::string script, Type type, T&&... argv)
{
	std::string params = get_query_type(type);

	std::string path = get_path();

	std::string q = exec + " " + path + "/" +
							script + " " + params +
							" " + create_query_param(std::forward<T>(argv)...);

	return q;
}

template<typename ...T>
std::string create_query(Type type, T&&... argv)
{
	std::string q = query("php", "decode.php", type,
	                          std::forward<T>(argv)...);
	return q;
}


std::string read(const std::string& path)
{
	std::string res;

	if(FILE* stream = fopen(path.c_str(), "rb"))
	{
		const int CAPACITY = 1024;
		char line[CAPACITY];

		while(fgets(line, CAPACITY, stream))
			res.append(line, strlen(line));

		fclose(stream);
	}

	return res;
}

void write(const std::string& path, const std::string& msg)
{
	if(FILE* stream = fopen(path.c_str(), "wb"))
	{
		fputs(msg.c_str(), stream);
		fclose(stream);
	}
}

void pipe_write(const std::string& query, const std::string& msg)
{
	if(FILE* pipe = popen(query.c_str(), "w"))
	{
		fputs(msg.c_str(), pipe);
		pclose(pipe);
	}
}

std::string pipe_read(const std::string& query)
{
	std::string res;

	if(FILE* pipe = popen(query.c_str(), "r"))
	{
		const int CAPACITY = 1024;
		char line[CAPACITY];

		while(fgets(line, CAPACITY, pipe))
			res.append(line, strlen(line));

		pclose(pipe);
	}

	return res;
}

std::string exec(Pipe pipe, const std::string& query, std::string msg = "")
{
	std::string res;

	switch (pipe)
	{
		case Pipe::READ:
			{
				res = pipe_read(query);
			}
		break;
		case Pipe::WRITE:
			{
				pipe_write(query, msg);
			}
		break;
	}

	return res;
}

std::string ssl_encode(const std::string& msg,
					   const std::string& publicKey)
{
	std::string res;
	std::string pipeline = "pipe";

	write(pipeline, msg);

	std::string query = create_query(Type::ENCODE, Type::PIPE, pipeline);

	exec(Pipe::WRITE, query, publicKey);

	res = read(pipeline);

	return res;
}

std::string ssl_decode(const std::string& msg,
					   const std::string& privateKey)
{
	std::string res;

	std::string pipeline = "pipe";

	write(pipeline, msg);

	std::string query = create_query(Type::DECODE, Type::PIPE, pipeline);

	exec(Pipe::WRITE, query, privateKey);

	res = read(pipeline);

	return res;
}

std::unordered_map<std::string, std::string> ssl_keys()
{
	std::unordered_map<std::string, std::string> keys;

	std::string query = create_query(Type::GENERATE);

	std::string key = exec(Pipe::READ, query);

	std::size_t beginPrivate = key.find("-----BEGIN PRIVATE KEY-----");
	std::size_t beginPublic = key.find("-----BEGIN PUBLIC KEY-----");

	keys.emplace("PRIVATE", key.substr(beginPrivate, beginPublic));

	keys.emplace("PUBLIC", key.substr(beginPublic));

	return keys;
}

int main()
{
	auto keys = ssl_keys();
	std::string encode = ssl_encode("Test", keys["PUBLIC"]);
	std::string decode = ssl_decode(encode, keys["PRIVATE"]);
	std::cout<<decode<<std::endl;

	return EXIT_SUCCESS;
}

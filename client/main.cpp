#include <iostream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <string>

namespace asio = boost::asio;
namespace json = boost::property_tree;

struct ServerAddr
{
	std::string addr;
	int port;
};

enum class RequestType
{
	AUTORIZATION = 1,
	MESSAGE = 2,
	HANDSHAKE = 3,
	GROUP_LIST = 4,
	GROUP_MESSAGES = 5
};

ServerAddr address = {"127.0.0.1", 7000};

std::string str_from_base64(const std::string& data)
{
	using namespace boost::archive::iterators;

	using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;

	return std::string(It(std::begin(data)), It(std::end(data)));
}

std::string str_to_base64(const std::string& str)
{
	using namespace boost::archive::iterators;
	using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
	return std::string(It(std::begin(str)), It(std::end(str)));
}

std::string read_ssl_key(std::string path)
{
	std::string key;
	std::ifstream stream(path, std::ios::binary);

	if(stream.is_open())
	{
		char buffer;
		while(stream.get(buffer))
		{
			key.push_back(buffer);
		}
	}

	stream.close();

	return key;
}

json::ptree create_json(std::map<std::string, std::string> map)
{
	json::ptree root;
	for(auto&& [key, value] : map)
	{
		root.put(key, value);
	}
	return root;
}

std::string get_string_json(const json::ptree& json)
{
	std::ostringstream stream;
	json::write_json(stream, json, false);

	return stream.str();
}

int main()
{
	std::string privKey = read_ssl_key("priv.txt");
	std::string pubKey = read_ssl_key("pub.txt");

	std::map<std::string, std::string> data;
	data.emplace("Type", "3");
	data.emplace("Key", pubKey);

	auto pt = create_json(data);

	asio::io_service service;

	asio::ip::tcp::socket socket(service);

	asio::ip::tcp::endpoint endPoint(asio::ip::address::from_string(address.addr), address.port);

	socket.connect(endPoint);

	asio::write(socket, asio::buffer(get_string_json(pt)));

	socket.wait(asio::socket_base::wait_read);

	asio::streambuf buffer;

	asio::read(socket, buffer, asio::transfer_at_least(socket.available()));

	json::ptree json;
	std::stringstream s;
	s << &buffer;
	json::read_json(s, json);

	std::cout<<json.get<std::string>("Key")<<std::endl;

	return EXIT_SUCCESS;
}

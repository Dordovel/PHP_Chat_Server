#include <iostream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <string>

#include "adapter/header/Ssl.h"
#include "header/Data.h"

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

json::ptree create_json(const std::map<std::string, std::string>& map)
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

asio::ip::tcp::socket connect(asio::io_service& service, ServerAddr address)
{
	asio::ip::tcp::socket socket(service);

	asio::ip::tcp::endpoint endPoint(asio::ip::address::from_string(address.addr), address.port);

	socket.connect(endPoint);

	return socket;
}

int main()
{
	Ssl ssl;
	auto rsaKeys = ssl.ssl_keys();
	std::string privKey = rsaKeys["PRIVATE"];
	std::string pubKey = rsaKeys["PUBLIC"];

	std::map<std::string, std::string> data;
	data.emplace("Type", str_to_base64("3"));
	data.emplace("Key", str_to_base64(pubKey));

	auto pt = create_json(data);

	json::ptree tree;
	json::ptree child;
	json::ptree chil1, chil2;
	chil1.put("name", "1");
	chil1.put("info", "2");
	chil1.put("creator", "3");

	chil2.put("name", "1");
	chil2.put("info", "2");
	chil2.put("creator", "3");

	child.push_back(std::make_pair("", chil1));
	child.push_back(std::make_pair("", chil2));

	tree.put("Type", "3");
	tree.add_child("Group", child);

	std::cout<<get_string_json(tree)<<std::endl;

	Data response;
	response.add_value("Type", "3");
	response.add_array("Group", "1", {std::make_pair("A", "B")});
	response.add_array("Group", "2", {std::make_pair("C", "D")});

	auto val = response.get_array("Group");

	for(auto outer : val)
	{
		std::cout<<outer.first<<std::endl;
		for(auto&& inner : outer.second)
		{
			std::cout<<inner.first<<"   :   "<<inner.second<<std::endl;
		}
	}

	std::cout<<get_string_json(response._json)<<std::endl;

	return EXIT_SUCCESS;

	asio::io_service service;

	asio::ip::tcp::socket socket = connect(service, address);

	asio::write(socket, asio::buffer(get_string_json(pt)));

	socket.wait(asio::socket_base::wait_read);

	asio::streambuf buffer;

	asio::read(socket, buffer, asio::transfer_at_least(socket.available()));

	json::ptree json;
	std::stringstream s;
	s << &buffer;
	json::read_json(s, json);

	std::cout<<str_from_base64(json.get<std::string>("Key"))<<std::endl;

	return EXIT_SUCCESS;
}

#include <iostream>
#include <boost/asio.hpp>
#include <string>

#include "adapter/header/Ssl.h"
#include "header/Data.h"

namespace asio = boost::asio;

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

	Data responce;
	responce.encode_value("Type", "3");
	responce.encode_value("Key", pubKey);

	asio::io_service service;

	asio::ip::tcp::socket socket = connect(service, address);

	asio::write(socket, asio::buffer(responce.get()));

	socket.wait(asio::socket_base::wait_read);

	asio::streambuf buffer;

	asio::read(socket, buffer, asio::transfer_at_least(socket.available()));

	std::stringstream s;
	s << &buffer;

	responce.set(s.str());

	std::cout<<"Server Key->"<<responce.decode_value("Key")<<std::endl;

	return EXIT_SUCCESS;
}

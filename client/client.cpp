#include <iostream>
#include <boost/asio.hpp>
#include <string>

#include "adapter/header/Ssl.h"
#include "header/Data.h"

#include "header/Client.h"

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
	Client connection(address.addr, address.port);
	connection.init_handshake();

	return EXIT_SUCCESS;

	Ssl ssl;
	auto rsaKeys = ssl.ssl_keys();
	std::string privKey = rsaKeys["PRIVATE"];
	std::string pubKey = rsaKeys["PUBLIC"];

	Json request;
	request.create("Type", ValueType::StringType) =  "3";
	request.create("Key", ValueType::StringType) =  pubKey;

	asio::io_service service;

	asio::ip::tcp::socket socket = connect(service, address);

	asio::write(socket, asio::buffer(Data(request).encode()));

	socket.wait(asio::socket_base::wait_read);

	asio::streambuf buffer;

	asio::read(socket, buffer, asio::transfer_at_least(socket.available()));

	std::stringstream s;
	s << &buffer;

	auto response = Data(s.str()).decode();

	std::cout << "Server Key->" << (*response)["Key"].str() << std::endl;

	return EXIT_SUCCESS;
}

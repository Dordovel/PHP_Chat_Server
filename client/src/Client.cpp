//
// Created by dordovel on 03.05.2021.
//

#include "../header/Client.h"
#include "../header/Connection.h"
#include "../Handlers/Handshake.h"
#include "../adapter/header/Ssl.h"

#include <iostream>

Client::Client(const std::string& addr, std::size_t port)
{
	this->_connection = std::make_unique<Connection>(addr, port);
}

void Client::init_handshake()
{
	Ssl ssl;
	auto clientKeys = ssl.ssl_keys();
	this->_connection->set_client_key(clientKeys["PUBLIC"]);
	this->_connection->set_client_private_key(clientKeys["PRIVATE"]);

	Handshake handshake;

	auto request = handshake.execute(this->_connection->get_client_key());
	this->_connection->write(request->get_data());
	this->_connection->wait_read();
	Data responce = this->_connection->read();
	std::string serverKey = handshake.handle(&responce);

	this->_connection->set_server_key(serverKey);

	std::cout<<"Server Key "<<serverKey<<std::endl;
}

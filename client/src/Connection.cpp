//
// Created by dordovel on 03.05.2021.
//

#include "../header/Connection.h"

#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>

Connection::Connection(const std::string& addr, std::size_t port):
				_context(std::make_unique<boost::asio::io_context>()),
				_socket(*this->_context)
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(addr), port);

	this->_socket.connect(endpoint);
}

void Connection::write(const std::string& data)
{
	boost::asio::write(this->_socket, boost::asio::buffer(data));
}

std::string Connection::read()
{
	boost::asio::streambuf buffer;
	boost::asio::read(this->_socket, buffer, boost::asio::transfer_at_least(this->_socket.available()));

	std::stringstream stream;
	stream << &buffer;

	return stream.str();
}

void Connection::wait_read()
{
	this->_socket.wait(boost::asio::socket_base::wait_read);
}

std::string Connection::get_server_key() const noexcept
{
	return this->_serverKey;
}

std::string Connection::get_client_key() const noexcept
{
	return this->_clientKey;
}

std::string Connection::get_client_private_key() const noexcept
{
	return this->_clientPrivateKey;
}

void Connection::set_server_key(std::string key) noexcept
{
	this->_serverKey = std::move(key);
}

void Connection::set_client_key(std::string key) noexcept
{
	this->_clientKey = std::move(key);
}

void Connection::set_client_private_key(std::string key) noexcept
{
	this->_clientPrivateKey = std::move(key);
}

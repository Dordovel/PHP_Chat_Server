//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>

#include "../interface/IConnection.h"

class Connection : public IConnection
{
	private:
		std::unique_ptr<boost::asio::io_context> _context;
		boost::asio::ip::tcp::socket _socket;

		std::string _serverKey;
		std::string _clientKey;
		std::string _clientPrivateKey;

	public:
		Connection(const std::string& addr, std::size_t port);

		std::string get_server_key() const noexcept override;
		std::string get_client_key() const noexcept override;
		std::string get_client_private_key() const noexcept override;

		void set_server_key(std::string key) noexcept override;
		void set_client_key(std::string key) noexcept override;
		void set_client_private_key(std::string key) noexcept override;

		void write(const std::string& data) override;
		std::string read() override;

		void wait_read() override;
};

#endif //CLIENT_CONNECTION_H

//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_ICONNECTION_H
#define CLIENT_ICONNECTION_H

#include <string>

class IConnection
{
	public:

		virtual std::string get_server_key() const noexcept = 0;
		virtual std::string get_client_key() const noexcept = 0;
		virtual std::string get_client_private_key() const noexcept = 0;

		virtual void set_server_key(std::string key) noexcept = 0;
		virtual void set_client_key(std::string key) noexcept = 0;
		virtual void set_client_private_key(std::string key) noexcept = 0;

		virtual void write(const std::string& data) = 0;
		virtual std::string read() = 0;

		virtual void wait_read() = 0;

		virtual ~IConnection() = default;
};

#endif //CLIENT_CLIENT_H

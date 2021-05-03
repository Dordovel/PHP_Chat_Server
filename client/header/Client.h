//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "../interface/IConnection.h"

#include <memory>

class Client
{
	private:
		std::unique_ptr<IConnection> _connection;
	public:
		Client(const std::string& addr, std::size_t port);

		void init_handshake();
};

#endif //CLIENT_CLIENT_H

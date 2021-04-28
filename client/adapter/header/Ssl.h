//
// Created by dordovel on 28.04.2021.
//

#ifndef CLIENT_SSL_H
#define CLIENT_SSL_H

#include <string>
#include <unordered_map>

#include "Query.h"
#include "File.h"
#include "Pipe.h"

class Ssl
{
	private:
		Query _query;
		File _file;
		Pipeline _pipe;

		std::string pipeline = "pipe";

		std::string ssl_encrypt(Query::Type type, const std::string& msg,
		                        const std::string& key);
	public:

		std::string ssl_encode(const std::string& msg,
		                       const std::string& publicKey);

		std::string ssl_decode(const std::string& msg,
		                       const std::string& privateKey);

		std::unordered_map<std::string, std::string> ssl_keys();
};


#endif //CLIENT_SSL_H

//
// Created by dordovel on 28.04.2021.
//

#include "../header/Ssl.h"


std::string Ssl::ssl_encrypt(Query::Type type, const std::string& msg,
                        const std::string& key)
{
	std::string res;

	this->_file.write(this->pipeline, msg);

	std::string query = this->_query.create_query(type, Query::Type::PIPE, pipeline);

	this->_pipe.exec(Pipeline::Type::WRITE, query, key);

	res = this->_file.read(this->pipeline);

	remove(this->pipeline.c_str());

	return res;
}

std::string Ssl::ssl_encode(const std::string& msg,
                       const std::string& publicKey)
{
	return ssl_encrypt(Query::Type::ENCODE, msg, publicKey);
}

std::string Ssl::ssl_decode(const std::string& msg,
                       const std::string& privateKey)
{
	return ssl_encrypt(Query::Type::DECODE, msg, privateKey);
}

std::unordered_map<std::string, std::string> Ssl::ssl_keys()
{
	std::unordered_map<std::string, std::string> keys;

	std::string query = this->_query.create_query(Query::Type::GENERATE);

	std::string key = this->_pipe.exec(Pipeline::Type::READ, query);

	std::size_t beginPrivate = key.find("-----BEGIN PRIVATE KEY-----");
	std::size_t beginPublic = key.find("-----BEGIN PUBLIC KEY-----");

	keys.emplace("PRIVATE", key.substr(beginPrivate, beginPublic));

	keys.emplace("PUBLIC", key.substr(beginPublic));

	return keys;
}

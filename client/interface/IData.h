//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <string>

class IData
{
	public:

		virtual std::string get_value(const std::string& key) const = 0;
		virtual Decoder::Array get_array(const std::string& key) const = 0;

		virtual void add_value(const std::string& key, const std::string& value) = 0;
		virtual void add_array(const std::string& key, const Encoder::Array& array) = 0;

		virtual ~IData() = default;
};

#endif //CLIENT_DATA_H

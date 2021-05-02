//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_DATA_H
#define EXECUTABLE_DATA_H


#include <string>
#include "Json.h"

class Data
{
	private:
		std::string _file;

		Json _json;

	public:
		Data();
		Data(std::string);

		std::string get() const noexcept;
		void set(std::string json) noexcept;

		std::string decode_value(const std::string& key);
		Decoder::Array decode_array(const std::string& key);

		void encode_value(const std::string& key, const std::string& value);
		void encode_array(const std::string& key, const Encoder::Array& array);
};

#endif //EXECUTABLE_DATA_H

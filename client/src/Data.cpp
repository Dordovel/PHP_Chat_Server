//
// Created by dordovel on 02.05.2021.
//

#include "Data.h"

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace
{
	std::string base64_decode(const std::string& data)
	{
		using namespace boost::archive::iterators;
		using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
		return std::string(It(std::begin(data)), It(std::end(data)));
	}

	std::string base64_encode(const std::string& str)
	{
		using namespace boost::archive::iterators;
		using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
		return std::string(It(std::begin(str)), It(std::end(str)));
	}

};

Data::Data()
{
	this->_file = this->_json.create();
}

Data::Data(std::string json): _file(std::move(json)) {}

std::string Data::get_data() const noexcept
{
	return this->_file;
}

void Data::set_data(std::string json) noexcept
{
	this->_file = std::move(json);
}

std::string Data::get_value(const std::string &key) const
{
	return base64_decode(this->_json.decode_value(this->_file, key));
}

Decoder::Array Data::get_array(const std::string &key) const
{
	Encoder::Array copy = this->_json.decode_array(this->_file, key);;

	for(auto& object : copy)
	{
		for(auto& field : object)
		{
			field.second = base64_decode(field.second);
		}
	}
	return copy;
}

void Data::add_value(const std::string &key, const std::string &value)
{
	decltype(value) copy = base64_encode(value);
	this->_file = this->_json.encode_value(this->_file, key, copy);
}

void Data::add_array(const std::string& key, const Encoder::Array &array)
{
	Encoder::Array copy = array;
	for(auto& object : copy)
	{
		for(auto& field : object)
		{
			field.second = base64_encode(field.second);
		}
	}

	this->_file = this->_json.encode_array(this->_file, key, copy);
}
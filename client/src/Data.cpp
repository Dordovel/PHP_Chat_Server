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

Data::Data(const std::string& json): _json(json) {}

Data::Data(Json json): _json(std::move(json)){}

std::string Data::encode() const noexcept
{
	return this->_json.get();
}

std::string Data::encode(const Json& json) const noexcept
{
	return json.get();
}

const Json* const Data::decode() const noexcept
{
	return &this->_json;
}

//
// Created by dordovel on 29.04.2021.
//

#include "../header/Data.h"

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <valarray>

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
}

namespace
{
	using namespace boost::property_tree;

	std::string json_to_string(const ptree& json)
	{
		std::ostringstream stream;
		write_json(stream, json, false);
		return stream.str();
	}

	ptree create_node(std::string_view key, std::string_view value)
	{
		ptree node;
		node.put(key.data(), value.data());
		return node;
	}

	ptree create_array(const std::vector<std::pair<std::string, std::string>>& value)
	{
		ptree node;
		for(auto&& val : value) node.put(val.first, base64_encode(val.second));
		return node;
	}
}

void Data::add_array(std::string_view nodePath,
					 std::string_view key,
					 const std::vector<std::pair<std::string, std::string>>& value) noexcept
{
	auto array = create_array(value);
	auto node = this->_json.get_child_optional(nodePath.data());
	if(!node)
	{
		ptree children;
		children.push_back(std::make_pair(key.data(), array));
		this->_json.add_child(nodePath.data(), children);
	}
	else node->push_back(std::make_pair(key.data(), array));
}

void Data::add_value(std::string_view key, std::string_view value) noexcept
{
	this->_json.put(key.data(), value.data());
}

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>  Data::get_array(std::string_view key) noexcept
{
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> res;

	if(auto find = this->_json.get_child_optional(key.data()); find)
	{
		for(auto&& val : *find)
		{
			decltype(res)::value_type::second_type second;

			for(auto&& s : val.second)
				second.emplace_back(s.first.data(), base64_decode(s.second.data()));

			res.emplace(val.first.data(), second);
		}
	}

	return res;
}

std::string Data::get_value(std::string_view key) noexcept
{
	return this->_json.get<std::string>(key.data());
}
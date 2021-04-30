//
// Created by dordovel on 29.04.2021.
//

#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <unordered_map>
#include <memory>
#include <vector>

#include <boost/property_tree/ptree.hpp>

class Data
{
	public:
		boost::property_tree::ptree _json;

	public:
		void add_array(std::string_view nodeKey,
				        std::string_view key,
				        const std::vector<std::pair<std::string, std::string>>& value) noexcept;

		void add_value(std::string_view key, std::string_view value) noexcept;

		std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> get_array(std::string_view key) noexcept;
		std::string get_value(std::string_view key) noexcept;
};


#endif //CLIENT_DATA_H

//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_ENCODER_H
#define EXECUTABLE_ENCODER_H

#include <unordered_map>
#include <vector>

class Encoder
{
	public:
		using Node = std::unordered_map<std::string, std::string>;
		using Array = std::vector<Node>;

		std::string encode_value(const std::string& json, const std::string& key, const std::string& value) const;
		std::string encode_array(const std::string& json, const std::string& key, const Array& array) const;
};

#endif //EXECUTABLE_ENCODER_H

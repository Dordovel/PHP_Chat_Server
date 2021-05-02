//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_DECODER_H
#define EXECUTABLE_DECODER_H

#include <unordered_map>
#include <vector>

class Decoder
{
	public:
		using Node = std::unordered_map<std::string, std::string>;
		using Array = std::vector<Node>;

		std::string decode_value(const std::string& json, const std::string& key);
		Decoder::Array decode_array(const std::string& json, const std::string& key);
};

#endif //EXECUTABLE_DECODER_H

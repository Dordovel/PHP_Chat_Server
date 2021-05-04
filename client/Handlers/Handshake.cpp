//
// Created by dordovel on 03.05.2021.
//

#include "Handshake.h"
#include "Types.h"


std::unique_ptr<IData> Handshake::execute(const std::string& key)
{
	Json json;
	json.create("Type", ValueType::StringType) = std::to_string(static_cast<int>(Type::HANDSHAKE));
	json.create("Key", ValueType::StringType) = key;

	std::unique_ptr<Data> data = std::make_unique<Data>(json);

	return data;
}

std::string Handshake::handle(const IData* const request)
{
	std::string res;

	const Json* const json = request->decode();

	if(!json->isset("Type") || !json->isset("Key"))
		return res;

	int value = (*json)["Type"].integer();

	Type type = static_cast<Type>(value);
	if(type == Type::HANDSHAKE)
		res = (*json)["Key"].str();

	return res;
}


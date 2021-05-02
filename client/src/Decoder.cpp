//
// Created by dordovel on 02.05.2021.
//

#include "../header/Decoder.h"

#include "../include/rapidjson/document.h"

namespace
{
	rapidjson::Document create_document(const std::string &json)
	{
		rapidjson::Document root;
		root.Parse(json.c_str());
		return root;
	}

	Decoder::Node decode_json_object(const rapidjson::Value &value)
	{
		using namespace rapidjson;

		Decoder::Node res;

		auto object = value.GetObject();

		for (const auto &field : object)
		{
			res.emplace(field.name.GetString(), field.value.GetString());
		}

		return res;
	}

	Decoder::Node decode_json_object_handle(const rapidjson::Value &value)
	{
		using namespace rapidjson;

		switch (value.GetType())
		{
			case Type::kObjectType:
				return decode_json_object(value);

			default:
				return {};
		}
	}

	Decoder::Array decode_json_array(const rapidjson::Value &value)
	{
		Decoder::Array res;

		auto array = value.GetArray();

		for (const auto &node : array)
			res.push_back(decode_json_object_handle(node));

		return res;
	}
}

Decoder::Array Decoder::decode_array(const std::string &json, const std::string &key)
{
	using namespace rapidjson;

	Document root = create_document(json);
	Value value(key.c_str(), key.size(), root.GetAllocator());

	if(!root.HasMember(value) || !root[value].IsArray())
		return {};

	return decode_json_array(root[value]);
}

std::string Decoder::decode_value(const std::string &json, const std::string &key)
{
	using namespace rapidjson;
	Document root = create_document(json);
	Value value(key.c_str(), key.size(), root.GetAllocator());

	if(!root.HasMember(value) || !root[value].IsString())
		return {};

	return root[value].GetString();
}
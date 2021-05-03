//
// Created by dordovel on 02.05.2021.
//

#include "Encoder.h"

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"

namespace
{
	using namespace rapidjson;

	Document create_document(const std::string &json)
	{
		Document root;
		root.Parse(json.c_str());
		return root;
	}

	std::string json_to_string(const Document& document)
	{
		StringBuffer buffer;
		buffer.Clear();

		Writer<decltype(buffer)> writer(buffer);
		document.Accept(writer);

		return std::string(buffer.GetString());
	}

	void add_value(Document& root, Value* key, Value* value)
	{
		root.AddMember(*key, *value, root.GetAllocator());
	}
};

std::string Encoder::encode_value(const std::string &json,
								  const std::string &key,
								  const std::string &value) const
{
	using namespace rapidjson;

	Document root = create_document(json);
	auto allocator = root.GetAllocator();

	Value first(key.c_str(), key.size(), allocator);
	Value second(value.c_str(), value.size(), allocator);

	add_value(root, &first, &second);

	return json_to_string(root);
}

std::string Encoder::encode_array(const std::string &json,
                                  const std::string &key,
								  const std::vector <std::unordered_map<std::string, std::string>> &array) const
{
	using namespace rapidjson;

	Document root = create_document(json);

	auto allocator = root.GetAllocator();

	Value cKey(key.c_str(), key.size(), root.GetAllocator());

	Value res(kArrayType);

	for(auto&& value : array)
	{
		Value buffer(kObjectType);

		for(auto&& val : value)
		{
			Value first(val.first.c_str(), val.first.size(), allocator);
			Value second(val.second.c_str(), val.second.size(), allocator);
			buffer.AddMember(first, second, allocator);
		}
		res.PushBack(buffer, allocator);
	}

	add_value(root, &cKey, &res);

	return json_to_string(root);

}

#include <unordered_map>
#include <iostream>

#include "header/Json.h"

void node_keys(const Node& node)
{
	auto keys = node.keys();
	for(auto&& key : keys)
	{
		std::cout<<"Node Key: "<<key<<std::endl;
		if(node.type() == ValueType::ArrayType)
			node_keys(node[std::stoi(key)]);
		else
			node_keys(node[key]);
	}
}

void document_keys(const Json& json)
{
	auto keys = json.keys();
	for(auto&& key : keys)
	{
		std::cout<<"Key: "<<key<<std::endl;
		node_keys(json[key]);
	}
}

int main()
{
	std::string jFile = "{\n\t\"name\": [{ \"first\": \"Name\"}], \"value\":\"Hello\" \n}";
	Json json(jFile);
	std::cout<<json.get()<<std::endl;

	Node node = json.create("array", ValueType::ArrayType);
	node = std::vector<std::string>{"1", "2", "3", "4"};

	Node::ObjectType array;
	array.emplace(std::make_pair("first", "First"));
	array.emplace(std::make_pair("second", "Second"));

	auto pointer = node.create(ValueType::ObjectType);
	pointer = array;
	std::cout<<node[4]["Second"].Str()<<std::endl;

	std::cout<<json.get()<<std::endl;
	return 0;
}

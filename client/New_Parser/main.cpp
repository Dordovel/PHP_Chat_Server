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
	Node node1 = json.create("node", ValueType::ArrayType);
	std::vector<std::unordered_map<std::string, std::string>> array;
	array.emplace_back(decltype(array)::value_type{std::make_pair("first", "First")});
	array.emplace_back(decltype(array)::value_type{std::make_pair("Second", "Second")});

	node1 = array;
	document_keys(json);
	std::cout<<json.get()<<std::endl;
	return 0;
}

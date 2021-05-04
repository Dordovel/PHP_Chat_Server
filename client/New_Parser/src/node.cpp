#include "../header/Json.h"
#include "../include/rapidjson/document.h"

#include<type_traits>

using namespace rapidjson;

namespace
{
	Value* cast(void* document)
	{
		return static_cast<Value*>(document);
	}

	MemoryPoolAllocator<CrtAllocator>* alloc(void* alloc)
	{
		return static_cast<MemoryPoolAllocator<CrtAllocator>*>(alloc);
	}
}

Node::Node(void* value, void* alloc)
{
	this->_value = value;
	this->_alloc = alloc;
}

Node Node::operator[](const std::string& key)
{
	Value* value = cast(this->_value);
	Node node(&(*value)[key.c_str()], this->_alloc);

	return node;
}

Node Node::operator[](int key)
{
	Value* value = cast(this->_value);
	Node node(&(*value)[key], this->_alloc);

	return node;
}

Node Node::operator[](const char* key)
{
	Value* value = cast(this->_value);
	Node node(&(*value)[key], this->_alloc);

	return node;
}


Node Node::operator[](const std::string& key) const
{
	Value* value = cast(this->_value);
	Node node(&(*value)[key.c_str()], this->_alloc);

	return node;
}

Node Node::operator[](int key) const
{
	Value* value = cast(this->_value);
	Node node(&(*value)[key], this->_alloc);

	return node;
}

Node Node::operator[](const char* key) const
{
	Value* value = cast(this->_value);
	Node node(&(*value)[key], this->_alloc);

	return node;
}

void Node::operator =(const std::string& value)
{
	Value* node = cast(this->_value);
	node->SetString(value.c_str(), value.size());
}

void Node::operator =(const std::vector<std::string>& data)
{
	Value* node = cast(this->_value);
	auto* allocator = alloc(this->_alloc);

	auto&& array = node->GetArray();

	for(auto& elem : data)
	{
		Value temp(elem.c_str(), elem.size(), *allocator);
		array.PushBack(temp, *allocator);
	}
}

void Node::operator =(const Node::List& data)
{
	Value* node = cast(this->_value);
	auto* allocator = alloc(this->_alloc);

	auto&& array = node->GetArray();

	for(auto& inner : data)
	{
		Value object(kObjectType);

		for(auto& elem : inner)
		{
			Value vKey(elem.first.c_str(), elem.first.size(), *allocator);
			Value vValue(elem.second.c_str(), elem.second.size(), *allocator);

			object.AddMember(vKey, vValue, *allocator);
		}

		array.PushBack(object, *allocator);
	}
}

std::string Node::str()
{
	Value* value = cast(this->_value);
	return value->GetString();
}

int Node::integer()
{
	Value* value = cast(this->_value);
	return value->GetInt();
}

std::unordered_map<std::string, std::string> get_object(const Value& value)
{
	std::unordered_map<std::string, std::string> res;

	auto object = value.GetObject();

	for(auto&& field : object)
		res.emplace(field.name.GetString(), field.value.GetString());

	return res;
}

std::vector<std::unordered_map<std::string, std::string>> Node::array()
{
	std::vector<std::unordered_map<std::string, std::string>> res;

	Value* value = cast(this->_value);
	auto array = value->GetArray();

	auto size = array.Size();
	for(decltype(size) i = 0; i < size; ++i)
	{
		if(array[i].IsObject())
		{
			res.emplace_back(get_object(array[i]));
		}
		else
		{
			auto f = std::make_pair(std::to_string(i), std::string(array[i].GetString()));
			res.emplace_back(decltype(res)::value_type{std::move(f)});
		}
	}

	return res;
}

ValueType Node::type() const
{
	return static_cast<ValueType>(cast(this->_value)->GetType());
}

std::vector<std::string> keys_array(Value* value)
{
	auto array = value->GetArray();
	std::vector<std::string> keys;

	auto size = array.Size();
	for(decltype(size) i = 0; i < size; ++i)
		keys.emplace_back(std::to_string(i));

	return keys;
}

std::vector<std::string> keys_object(Value* value)
{
	auto object = value->GetObject();
	std::vector<std::string> keys;

	for(auto& val : object)
		keys.emplace_back(val.name.GetString());

	return keys;
}

std::vector<std::string> Node::keys() const
{
	Value* value = cast(this->_value);

	std::vector<std::string> keys;

	if(value->IsArray())
		keys = keys_array(value);
	else if(value->IsObject())
		keys = keys_object(value);

	return keys;
}

Node Node::create(const std::string& key, ValueType type)
{
	Value* value = cast(this->_value);

	auto allocator = alloc(this->_alloc);

	Value vKey(key.c_str(), key.size(), *allocator);

	Value vValue(rapidjson::Type(static_cast<int>(type)));

	value->AddMember(vKey, vValue, *allocator);

	return (*this)[key];
}

bool Node::isset(const std::string& key) const
{
	return cast(this->_value)->HasMember(key.c_str());
}

Node::~Node()
{
	this->_value = nullptr;
}

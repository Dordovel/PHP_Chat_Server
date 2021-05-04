//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_JSON_H
#define EXECUTABLE_JSON_H

#include <string>
#include <memory>

#include <unordered_map>
#include <vector>

enum class ValueType
{
    ObjectType = 3,
    ArrayType = 4,
    StringType = 5,
};

class Node
{
	private:
		void* _value;
		void* _alloc;

	public:

		using ObjectType = std::unordered_map<std::string, std::string>;
		using ArrayType = std::vector<std::string>;
		using StringType = std::string;

		Node(void* value, void* alloc);
		Node operator[](const std::string& key);
		Node operator[](const char* key);
		Node operator[](int key);

		Node operator[](const std::string& key) const;
		Node operator[](const char* key) const;
		Node operator[](int key) const;

		void operator =(const Node::StringType& value);
		void operator =(const Node::ArrayType& data);
		void operator =(const Node::ObjectType& data);

		std::string Str();
		int Int();
		ObjectType Object();
		ValueType type() const;

		std::vector<std::string> keys() const;

		Node create(const std::string& key, ValueType type);
		Node create(ValueType type);
		bool isset(const std::string& key) const;
		~Node();
};


class Json
{
	private:
		void* _document;
	public:
		Json();
		Json(const std::string& json);

		Json(const Json&) = default;
		Json& operator = (const Json&) = default;

		Json(Json&& json);
		Json& operator = (Json&& json);

		Node operator [](const std::string& key);
		Node operator[](const char* key);

		Node operator [](const std::string& key) const;
		Node operator[](const char* key) const;

		std::vector<std::string> keys() const;

		bool isset(const std::string& key) const;
		Node create(const std::string& key, ValueType type);
		std::string get() const;
		~Json();
};


#endif //EXECUTABLE_JSON_H

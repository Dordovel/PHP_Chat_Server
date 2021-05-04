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

		using List = std::vector<std::unordered_map<std::string, std::string>>;

	public:
		Node(void* value, void* alloc);
		Node operator[](const std::string& key);
		Node operator[](const char* key);
		Node operator[](int key);

		Node operator[](const std::string& key) const;
		Node operator[](const char* key) const;
		Node operator[](int key) const;

		void operator =(const std::string& value);
		void operator =(const std::vector<std::string>& data);
		void operator =(const Node::List& data);

		std::string str();
		int integer();
		List array();
		ValueType type() const;

		std::vector<std::string> keys() const;

		Node create(const std::string& key, ValueType type);
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

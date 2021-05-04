//
// Created by dordovel on 02.05.2021.
//

#include "../header/Json.h"

#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"

using namespace rapidjson;

namespace
{
	Document* cast(void* document)
	{
		return static_cast<Document*>(document);
	}
}

Json::Json()
{
	Document* document = new Document;
	document->SetObject();

	this->_document = static_cast<void*>(document);
}

Json::Json(const std::string& json)
{
	Document* document = new Document;
	document->Parse(json.c_str());

	this->_document = static_cast<void*>(document);

	document = nullptr;
}

Json::Json(Json&& json)
{
	this->_document = json._document;
	json._document = nullptr;
}

Json& Json::operator = (Json&& json)
{
	this->_document = json._document;
	json._document = nullptr;

	return *this;
}

Node Json::operator[](const std::string& key)
{
	Document* document = cast(this->_document);
	Node node(&(*document)[key.c_str()], &document->GetAllocator());

	return node;
}

Node Json::operator[](const char* key)
{
	Document* document = cast(this->_document);
	Node node(&(*document)[key], &document->GetAllocator());

	return node;
}

Node Json::operator[](const std::string& key) const
{
	Document* document = cast(this->_document);
	Node node(&(*document)[key.c_str()], &document->GetAllocator());

	return node;
}

Node Json::operator[](const char* key) const
{
	Document* document = cast(this->_document);
	Node node(&(*document)[key], &document->GetAllocator());

	return node;
}

std::vector<std::string> Json::keys() const
{
	Document* document = cast(this->_document);

	std::vector<std::string> keys;

	for(auto&& iter = document->MemberBegin(); iter != document->MemberEnd(); ++iter)
		keys.emplace_back(iter->name.GetString());

	return keys;
}

bool Json::isset(const std::string& key) const
{
	return cast(this->_document)->HasMember(key.c_str());
}

Node Json::create(const std::string& key, ValueType type)
{
	Document* document = cast(this->_document);

	auto* alloc = &document->GetAllocator();

	Value vKey(key.c_str(), key.size(), *alloc);

	Value vValue(rapidjson::Type(static_cast<int>(type)));

	document->AddMember(vKey, vValue, *alloc);

	return (*this)[key];
}

std::string Json::get() const
{
	StringBuffer buffer;
	buffer.Clear();

	Writer<decltype(buffer)> writer(buffer);
	cast(this->_document)->Accept(writer);

	return std::string(buffer.GetString());
}

Json::~Json()
{
	Document* document = cast(this->_document);
	if(document) delete document;

	this->_document = nullptr;
}

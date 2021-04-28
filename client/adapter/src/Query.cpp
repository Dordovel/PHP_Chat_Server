//
// Created by dordovel on 28.04.2021.
//

#include "../header/Query.h"

std::string Query::get_value(const std::string& value)
{
	return value;
}

std::string Query::get_value(Type type)
{
	return this->get_query_type_str(type);
}

std::string Query::get_query_type_str(Query::Type type)
{
	std::string params;

	switch(type)
	{
		case Query::Type::DECODE:
			params = "--decode";
			break;
		case Query::Type::ENCODE:
			params = "--encode";
			break;
		case Query::Type::GENERATE:
			params = "--generate";
			break;
		case Query::Type::KEY:
			params = "--key";
			break;
		case Query::Type::PIPE:
			params = "--pipe";
			break;
		default: break;
	}

	return params;
}

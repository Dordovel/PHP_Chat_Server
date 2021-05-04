//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include <string>

class Json;

class IData
{
	public:

		virtual std::string encode() const noexcept = 0;
		virtual const Json* const decode() const noexcept = 0;
		virtual ~IData() = default;
};

#endif //CLIENT_DATA_H

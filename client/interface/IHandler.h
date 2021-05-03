//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include "IData.h"
#include "IConnection.h"
#include "IGetter.h"

#include <memory>

struct Responce
{
	bool status;
	std::unique_ptr<IGetter> getter;
};

class IHandler
{
	public:
		virtual Responce execute(const IConnection* const data) = 0;

		virtual ~IHandler() = default;
};

#endif //CLIENT_HANDLER_H

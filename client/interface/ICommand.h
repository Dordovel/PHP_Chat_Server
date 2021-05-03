//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include "IGetter.h"
#include "IConnection.h"

#include <memory>

class Command
{
	public:
		virtual std::unique_ptr<IGetter> execute(const IConnection& connection) = 0;

		virtual ~Command() = default;
};

#endif //CLIENT_COMMAND_H

//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_HANDSHAKE_H
#define CLIENT_HANDSHAKE_H


#include "../header/Data.h"
#include <memory>

class Handshake
{
	public:
		std::unique_ptr<IData> execute(const std::string& key);
		std::string handle(const IData* const request);
};

#endif //CLIENT_HANDSHAKE_H

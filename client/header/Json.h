//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_JSON_H
#define EXECUTABLE_JSON_H

#include "Decoder.h"
#include "Encoder.h"

class Json : public Decoder, public Encoder
{
	public:
		std::string create();
};


#endif //EXECUTABLE_JSON_H

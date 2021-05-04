//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_DATA_H
#define EXECUTABLE_DATA_H


#include <string>
#include "../New_Parser/header/Json.h"

#include "../interface/IData.h"

class Data : public IData
{
	private:
		Json _json;

	public:
		Data() = default;
		Data(const std::string& json);
		Data(Json json);

		std::string encode() const noexcept override;
		std::string encode(const Json& json) const noexcept;
		const Json* const decode() const noexcept override;
};

#endif //EXECUTABLE_DATA_H

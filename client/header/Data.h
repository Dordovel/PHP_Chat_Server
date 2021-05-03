//
// Created by dordovel on 02.05.2021.
//

#ifndef EXECUTABLE_DATA_H
#define EXECUTABLE_DATA_H


#include <string>
#include "Json.h"

#include "../interface/IData.h"
#include "../interface/IGetter.h"

class Data : public IData, public IGetter
{
	private:
		std::string _file;

		Json _json;

	public:
		Data();
		Data(std::string);

		std::string get_data() const noexcept override;
		void set_data(std::string json) noexcept;

		std::string get_value(const std::string& key) const override;
		Decoder::Array get_array(const std::string& key) const override;

		void add_value(const std::string& key, const std::string& value) override;
		void add_array(const std::string& key, const Encoder::Array& array) override;
};

#endif //EXECUTABLE_DATA_H

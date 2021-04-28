//
// Created by dordovel on 28.04.2021.
//

#ifndef CLIENT_PIPE_H
#define CLIENT_PIPE_H

#include "./File.h"

class Pipeline
{
	private:
		File _file;

		void pipe_write(const std::string& query, const std::string& msg);
		std::string pipe_read(const std::string& query);

	public:
		enum class Type
		{
			READ,
			WRITE
		};

		std::string exec(Type pipe, const std::string& query, std::string msg = "");
};


#endif //CLIENT_PIPE_H

//
// Created by dordovel on 28.04.2021.
//

#include "../header/Pipe.h"

void Pipeline::pipe_write(const std::string& query, const std::string& msg)
{
	if(FILE* pipe = popen(query.c_str(), "w"))
	{
		fputs(msg.c_str(), pipe);
		pclose(pipe);
	}
}

std::string Pipeline::pipe_read(const std::string& query)
{
	std::string res;

	if(FILE* pipe = popen(query.c_str(), "r"))
	{
		res = this->_file.read_from_fd(pipe);
		pclose(pipe);
	}

	return res;
}

std::string Pipeline::exec(Pipeline::Type pipe, const std::string& query, std::string msg)
{
	std::string res;

	switch (pipe)
	{
		case Type::READ:
		{
			res = this->pipe_read(query);
		}
			break;
		case Type::WRITE:
		{
			this->pipe_write(query, msg);
		}
			break;
	}

	return res;
}

//
// Created by dordovel on 28.04.2021.
//

#ifndef CLIENT_FILE_H
#define CLIENT_FILE_H

#include <string>

class File
{
	public:

		std::string read_from_fd(FILE* fd);

		std::string read(const std::string& path);

		void write(const std::string& path, const std::string& msg);

		std::string get_path();
};


#endif //CLIENT_FILE_H

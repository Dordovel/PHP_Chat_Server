//
// Created by dordovel on 28.04.2021.
//

#include "../header/File.h"

#include <unistd.h>
#include <libgen.h>
#include <cstring>

std::string File::get_path()
{
	std::string path;

	char buffer[2048];

	if(readlink("/proc/self/exe", buffer, sizeof buffer) > 0)
		path = dirname(buffer);

	return path;
}

std::string File::read_from_fd(FILE* fd)
{
	std::string res;

	const int CAPACITY = 1024;
	char line[CAPACITY];

	while(fgets(line, CAPACITY, fd))
		res.append(line, strlen(line));

	return res;
}

std::string File::read(const std::string& path)
{
	std::string res;

	if(FILE* stream = fopen(path.c_str(), "rb"))
	{
		res = read_from_fd(stream);
		fclose(stream);
	}

	return res;
}

void File::write(const std::string& path, const std::string& msg)
{
	if(FILE* stream = fopen(path.c_str(), "wb"))
	{
		fputs(msg.c_str(), stream);
		fclose(stream);
	}
}

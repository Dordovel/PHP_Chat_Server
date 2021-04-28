//
// Created by dordovel on 28.04.2021.
//

#ifndef CLIENT_QUERY_H
#define CLIENT_QUERY_H

#include <tuple>
#include "File.h"

class Query
{
	public:
		enum class Type
		{
			ENCODE,
			DECODE,
			GENERATE,
			KEY,
			PIPE
		};

	private:
		File _file;

		std::string get_value(const std::string& value);

		std::string get_value(Query::Type type);

		template<typename std::size_t I = 0,
				typename ...Argv>
		typename std::enable_if<I == sizeof...(Argv), std::string>::type
		unpack(std::tuple<Argv...> argv, const std::string& separator = " ")
		{
			return "";
		}

		template<typename std::size_t I = 0,
				typename ...Argv>
		typename std::enable_if<I < sizeof...(Argv), std::string>::type
		unpack(std::tuple<Argv...> argv, const std::string& separator = " ")
		{
			std::string res = get_value(std::get<I>(argv));
			if(I < (sizeof...(Argv) - 1)) res += separator;
			res += get_value(unpack<I+1>(argv));

			return res;
		}

		template<typename ...T>
		std::string create_query_param(T&&... argv)
		{
			return unpack(std::make_tuple(argv...));
		}

		template<typename ...T>
		std::string query(std::string exec, std::string script, Query::Type type, T&&... argv)
		{
			std::string params = get_query_type_str(type);

			std::string path = this->_file.get_path();

			std::string q = exec + " " + path + "/" +
			                script + " " + params +
			                " " + create_query_param(std::forward<T>(argv)...);

			return q;
		}

	public:

		std::string get_query_type_str(Query::Type type);

		template<typename ...T>
		std::string create_query(Query::Type type, T&&... argv)
		{
			std::string q = query("php", "decode.php", type,
			                      std::forward<T>(argv)...);
			return q;
		}
};


#endif //CLIENT_QUERY_H

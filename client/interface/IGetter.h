//
// Created by dordovel on 03.05.2021.
//

#ifndef CLIENT_REQUEST_H
#define CLIENT_REQUEST_H

class IGetter
{
	public:
		virtual std::string get_data() const noexcept = 0;

		virtual ~IGetter() = default;
};

#endif //CLIENT_REQUEST_H

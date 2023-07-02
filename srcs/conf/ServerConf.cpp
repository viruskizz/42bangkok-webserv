#pragma once
#include <iostream>

class ServerConf
{
private:
	/* data */
public:
	ServerConf();
	~ServerConf();

	int			listen;
	std::string	serverName;
	std::string	root;
};

ServerConf::ServerConf() {}

ServerConf::~ServerConf() {}

std::ostream & operator << (std::ostream & o, ServerConf const & rhs) {
	// return o << rhs.getFiledata();
	return o;
}

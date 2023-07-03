#pragma once
#include <iostream>

class ServerConf
{
private:
	/* data */
	ServerConf();
public:
	~ServerConf();

	int			listen;
	std::string	serverName;
	std::string	root;
};

std::ostream & operator << (std::ostream &, ServerConf const &);

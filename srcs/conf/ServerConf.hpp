#pragma once
#include <iostream>

class ServerConf
{
private:
	/* data */
public:
	ServerConf(void);
	~ServerConf(void);

	int			listen;
	std::string	serverName;
	std::string	root;
};

std::ostream & operator << (std::ostream &, ServerConf const &);

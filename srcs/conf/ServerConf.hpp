#pragma once
#include <iostream>

class ServerConf
{
private:
	/* data */
public:
	ServerConf(void);
	~ServerConf(void);

	// * change to char *
	// char	*listen;
	// char	*serverName;
	// char	*root;
	std::string	listen;
	std::string	serverName;
	std::string	root;

};

std::ostream & operator << (std::ostream &, ServerConf const &);

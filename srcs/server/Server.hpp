#pragma once

#include "../Webserv.hpp"
#include "../conf/Config.hpp"
#include "./ServerRespond.hpp"

# define PORT "8080"
# define BACK_LOG 10

class Server
{
private:
	/* data */
	struct addrinfo m_hint;
	struct addrinfo *m_addrList;
	int				m_socketFd;
	std::vector<int>	m_serverSockets;
	std::vector<struct addrinfo *> m_address;
	// int				m_maxSocketFd;
	Config config;

public:
	Server();
	~Server();

	std::vector<int> getServerSockets(void);
	std::vector<struct addrinfo *> getAddress(void);

	void static exitWithError(char *errorMessage, int mode);
	void init(void);
	void start(void);
};

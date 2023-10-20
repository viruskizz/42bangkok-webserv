#pragma once

#include "../Webserv.hpp"
#include "../conf/Config.hpp"
#include "./HttpRespond.hpp"

# define PORT "8080"
# define BACK_LOG 100

class Server
{
private:
	/* data */
	struct addrinfo m_hint;
	struct addrinfo *m_addrList;
	int				m_socketFd;
	std::vector<int>	m_serverSockets;
	std::vector<struct addrinfo *> m_address;
	Config config;

	bool	cilentAccept(fd_set currentSocket, int maxSocketFd, int socket);
	void	sentRespond(Config const &configFile, fd_set currentSocket, int socket);

public:
	Server();
	~Server();

	std::vector<int> getServerSockets(void);
	std::vector<struct addrinfo *> getAddress(void);

	// void static exitWithError(char *errorMessage, int mode);
	void init(Config const &configFile);
	void start(Config const &configFile);
};

#include "Server.hpp"

Server::Server() {}

Server::~Server() {}

std::vector<int> Server::getServerSockets()
{
	return m_serverSockets;
}

std::vector<struct addrinfo *> Server::getAddress()
{
	return m_address;
}

void Server::exitWithError(char *errorMessage, int mode)
{
	if (mode == EE_PERR)
		perror(errorMessage);
	else if (mode == EE_NONE)
		std::cerr << errorMessage << std::endl;
	exit(EXIT_FAILURE);
}

void Server::init(Config const &configFile)
{
	int errorNumber;
	int	optionInput;

	for (int index = 0; index < configFile.getServers().size(); ++index)
	{
		memset(&m_hint, '\0', sizeof(struct addrinfo));
		m_hint.ai_family = AF_UNSPEC; // * allow ip address AF_INET ipv4, AF_INET6 ipv6 ;
		m_hint.ai_socktype = SOCK_STREAM;
		m_hint.ai_flags = AI_PASSIVE;

		errorNumber = getaddrinfo(/*configFile.getServers().at(index)->getServerName().c_str()*/0,
			configFile.getServers().at(index)->getListen().c_str(), &m_hint, &m_addrList);
		if (errorNumber != 0)
			Server::exitWithError(const_cast<char *>(gai_strerror(errorNumber)), EE_PERR); // TODO checkerror output;
		for (struct addrinfo *ptr = m_addrList;; ptr = ptr->ai_next)
		{
			if (ptr == NULL)
				exitWithError((char *)"webserv: ERROR: ", EE_PERR);

			// * open the socket for web server;
			m_socketFd = socket(ptr->ai_family, ptr->ai_socktype, 0);
			if (m_socketFd < 1)
				continue;

			// * set option for socket;
			optionInput = 1;
			if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR ,&optionInput, ptr->ai_addrlen))
				exitWithError((char *)"wevserv: ERROR ", EE_PERR);
			if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEPORT ,&optionInput, ptr->ai_addrlen))
				exitWithError((char *)"wevserv: ERROR ", EE_PERR);

			// * bind socket with value in struct sockaddr_in;
			if (bind(m_socketFd, ptr->ai_addr, ptr->ai_addrlen) == 0)
			{
				fcntl(m_socketFd, F_SETFL, O_NONBLOCK);
				m_serverSockets.push_back(m_socketFd);
				m_address.push_back(ptr);
				break;
			}
		}
		if (listen(m_socketFd, BACK_LOG) < 0) // TODO change backlog with config value
			Server::exitWithError((char *)"webserv: ERROR: ", EE_PERR);
	}
}

void Server::start(Config const &configFile)
{
	int				clientSocket;
	struct timeval	timeOut;
	fd_set			currentSocket;
	fd_set			readySocket;

	timeOut.tv_sec = 1;
	timeOut.tv_usec = 0;
	FD_ZERO(&currentSocket);
	for (int i = 0; i < m_serverSockets.size(); ++i)
	{
		FD_SET(m_serverSockets[i], &currentSocket);
	}
	int maxSocketFd = m_serverSockets.back() + 1;
	while (true)
	{
		readySocket = currentSocket;

		// std::cout << "********** Waiting for Client Request **********" << std::endl;
		if (select(maxSocketFd, &readySocket, NULL, NULL, &timeOut) < 0)
			Server::exitWithError((char *)"webserve: ERROR", EE_PERR);
		// std::cout << "********** Client Sent Some Request **********" << std::endl;
		for (int socket = 0; socket < maxSocketFd; ++socket)
		{
			if (FD_ISSET(socket, &readySocket))
			{
				bool isFound = false;
				for (int j = 0; j < m_serverSockets.size(); ++j)
				{
					int jsocket = m_serverSockets[j];
					if (jsocket == socket)
					{
						// std::cout << "********** Accept Client Request ***********" << std::endl;
						struct addrinfo *address = m_address[j];
						clientSocket = accept(jsocket, address->ai_addr, &address->ai_addrlen);
						fcntl(clientSocket, F_SETFL, O_NONBLOCK);
						if (clientSocket < 0)
							Server::exitWithError((char *)"webserv: ERROR: ", EE_PERR);
						FD_SET(clientSocket, &currentSocket);
						if (clientSocket > maxSocketFd - 1)
							maxSocketFd = clientSocket + 1;
						isFound = true;
						// std::cout << "********** Accept Client Done **********" << std::endl;
					}
				}
				if (!isFound)
				{
					// std::cout << "********** Sending Respond to Client **********" << std::endl;
					fcntl(socket, F_SETFL, O_NONBLOCK);
					HttpRequest request(socket, configFile);
					if (request.getRequestHeader().size())
					{
						if (request.getRequestBody().size() && request.getFileCGI().empty() && request.getRequestHeader().at("Method") != "PUT")
						{
							std::cout << "[Debug] ooooo" << std::endl;
							for (int count = 0; count < request.getRequestBody().size(); ++count)
							{
								HttpRespond respond(socket, request, configFile, count);
								respond.sendRepond(socket);
							}
						}
						else
						{
							std::cout << "[Debug] hello" << std::endl;
							HttpRespond respond(socket, request, configFile, 0);
							respond.sendRepond(socket);
						}
						
					}
					// std::cout << respond << std::endl;
					close(socket);
					FD_CLR(socket, &currentSocket);
					// std::cout << "********** Sent Respond to Client **********" << std::endl;
				}
			}
		}
	}
}

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

void Server::init(Config const &configFile)
{
	int errorNumber;
	int	optionInput;

	for (size_t index = 0; index < configFile.getServers().size(); ++index)
	{
		memset((void *)&m_hint, '\0', sizeof(struct addrinfo));
		m_hint.ai_family = AF_UNSPEC;
		m_hint.ai_socktype = SOCK_STREAM;
		m_hint.ai_flags = AI_PASSIVE;
		errorNumber = getaddrinfo(0, configFile.getServers().at(index)->getListen().c_str(), &m_hint, &m_addrList);
		if (errorNumber != 0)
			exitWithError(const_cast<char *>(gai_strerror(errorNumber)), EE_PERR);
		for (struct addrinfo *ptr = m_addrList;; ptr = ptr->ai_next)
		{
			if (ptr == NULL)
				exitWithError((char *)"webserv: ERROR: ", EE_PERR);
			m_socketFd = socket(ptr->ai_family, ptr->ai_socktype, 0);
			if (m_socketFd < 1)
				continue;
			optionInput = 1;
			if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR ,&optionInput, ptr->ai_addrlen))
				exitWithError((char *)"wevserv: ERROR ", EE_PERR);
			if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEPORT ,&optionInput, ptr->ai_addrlen))
				exitWithError((char *)"wevserv: ERROR ", EE_PERR);
			if (bind(m_socketFd, ptr->ai_addr, ptr->ai_addrlen) == 0)
			{
				fcntl(m_socketFd, F_SETFL, O_NONBLOCK);
				m_serverSockets.push_back(m_socketFd);
				m_address.push_back(ptr);
				break;
			}
		}
		if (listen(m_socketFd, BACK_LOG) < 0)
			exitWithError((char *)"webserv: ERROR: ", EE_PERR);
	}
}

void Server::start(Config const &configFile)
{
	int 			maxSocketFd;
	fd_set			currentSocket;
	fd_set			readySocket;

	FD_ZERO(&currentSocket);
	for (size_t i = 0; i < m_serverSockets.size(); ++i)
		FD_SET(m_serverSockets[i], &currentSocket);
	maxSocketFd = m_serverSockets.back() + 1;
	while (true)
	{
		readySocket = currentSocket;
		if (select(maxSocketFd, &readySocket, NULL, NULL, NULL) < 0)
			exitWithError((char *)"webserve: ERROR", EE_PERR);
		for (int socket = 0; socket < maxSocketFd; ++socket)
		{
			if (FD_ISSET(socket, &readySocket))
			{
				if(!this->cilentAccept(currentSocket, maxSocketFd, socket))
					sentRespond(configFile, currentSocket, socket);
			}
		}
	}
}

bool	Server::cilentAccept(fd_set &currentSocket, int &maxSocketFd, int socket)
{
	bool	isFound;
	int		clientSocket;
	int 	jsocket;
	
	isFound = false;
	for (size_t count = 0; count < m_serverSockets.size(); ++count)
	{
		jsocket = m_serverSockets[count];
		if (jsocket == socket)
		{
			struct addrinfo *address = m_address[count];
			clientSocket = accept(jsocket, address->ai_addr, &address->ai_addrlen);
			fcntl(clientSocket, F_SETFL, O_NONBLOCK);
			if (clientSocket < 0)
				exitWithError((char *)"webserv: ERROR: ", EE_PERR);
			FD_SET(clientSocket, &currentSocket);
			if (clientSocket > maxSocketFd - 1)
				maxSocketFd = clientSocket + 1;
			isFound = true;
		}
	}
	return (isFound);
}

void	Server::sentRespond(Config const &configFile, fd_set &currentSocket, int socket)
{
	fcntl(socket, F_SETFL, O_NONBLOCK);
	HttpRequest request(socket, configFile);
	if (request.getRequestHeader().size())
	{
		if (request.getRequestBody().size() && request.getFileCGI().empty() && request.getRequestHeader().at("Method") != "PUT")
		{
			for (size_t count = 0; count < request.getRequestBody().size(); ++count)
			{
				HttpRespond respond(request, configFile, count);
				respond.sendRepond(socket);
			}
		}
		else
		{
			HttpRespond respond(request, configFile, 0);
			respond.sendRepond(socket);
		}
	}
	close(socket);
	FD_CLR(socket, &currentSocket);
}

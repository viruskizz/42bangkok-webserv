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

void Server::init()
{
	int errorNumber;
	Config *configFile = new Config();

	memset(&m_hint, '\0', sizeof(struct addrinfo));
	m_hint.ai_family = AF_UNSPEC; // * allow ip address AF_INET ipv4, AF_INET6 ipv6 ;
	m_hint.ai_socktype = SOCK_STREAM;
	m_hint.ai_flags = AI_PASSIVE;

	errorNumber = getaddrinfo("127.0.0.1", configFile->getServer()[0]->listen.c_str(), &m_hint, &m_addrList); // * << change position 0 to i for dynamic;
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

		// * bind socket with value in struct sockaddr_in;
		if (bind(m_socketFd, ptr->ai_addr, ptr->ai_addrlen) == 0)
		{
			// fcntl(socketFd, F_SETFL, O_NONBLOCK);
			m_serverSockets.push_back(m_socketFd);
			m_address.push_back(ptr);
			break;
		}
	}
	if (listen(m_socketFd, BACK_LOG) < 0) // TODO change backlog with config value
		Server::exitWithError((char *)"webserv: ERROR: ", EE_PERR);
}

void Server::start(void)
{
	Config *configFile = new Config();
	int		clientSocket;
	fd_set	currentSocket;
	fd_set	readySocket;

	FD_ZERO(&currentSocket);
	for (int i = 0; i < m_serverSockets.size(); ++i)
	{
		FD_SET(m_serverSockets[i], &currentSocket);
	}
	int maxSocketFd = m_serverSockets.back() + 1;
	while (true)
	{
		readySocket = currentSocket;

		std::cout << "********** Waiting for Client Request **********" << std::endl;
		if (select(maxSocketFd, &readySocket, NULL, NULL, NULL) < 0)
			Server::exitWithError((char *)"webserve: ERROR", EE_PERR);
		std::cout << "********** Client Sent Some Request **********" << std::endl;
		for (int socket = 0; socket < maxSocketFd; ++socket)
		{
			// std::cout << "hello in FD_ISSET i = " << i << std::endl;
			if (FD_ISSET(socket, &readySocket))
			{
				// std::cout << "checkFd |" << i << "| in serverSocket" << std::endl;
				bool isFound = false;
				for (int j = 0; j < m_serverSockets.size(); ++j)
				{
					int jsocket = m_serverSockets[j];
					if (jsocket == socket)
					{
						std::cout << "********** Accept Client Request ***********" << std::endl;
						struct addrinfo *address = m_address[j];
						clientSocket = accept(jsocket, address->ai_addr, &address->ai_addrlen);
						// fcntl(clientSocket, F_SETFL, O_NONBLOCK);
						if (clientSocket < 0)
							Server::exitWithError((char *)"webserv: ERROR: ", EE_PERR);
						FD_SET(clientSocket, &currentSocket);
						if (clientSocket > maxSocketFd - 1)
							maxSocketFd = clientSocket + 1;
						isFound = true;
						std::cout << "********** Accept Client Done **********" << std::endl;
					}
					// std::cout << "hello in isFound loop | j = " << j << " | serverSocker[j] : " << serverSocket[j] << std::endl;
				}
				if (isFound)
				{
					std::cout << "********** Sending Respond to Client **********" << std::endl;
					fcntl(socket, F_SETFL, O_NONBLOCK);
					ServerRequest request = Request();
					// config.map(request);
					ServerRespond respond(socket, *configFile);
					respond.sendRepond(socket);
					// write(socket, respond.getRespond().c_str(), respond.getRespondLength());
					// std::cout << respond << std::endl;
					close(socket);
					// serverHandler(i, *configFile);
					FD_CLR(socket, &currentSocket);
					std::cout << "********** Sent Respond to Client **********" << std::endl;
				}
			}
		}
	}
}

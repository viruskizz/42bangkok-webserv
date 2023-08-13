#include "Webserv.hpp"
#include "./server/ServerRespond.hpp"
#include "./server/RequestHeader.hpp"
// #include <netdb.h>

std::vector<int>				serverSocket;
std::vector<struct addrinfo *>	address;
// Config							*configFile;

# define PORT "8080"
# define BACK_LOG 10

static void exitWithError(char *errorMessage, int mode)
{
	if (mode == EE_PERR)
		perror(errorMessage);
	else if (mode == EE_NONE)
		std::cerr << errorMessage << std::endl;
	exit (EXIT_FAILURE);
}


static void	signalHandle(int signalNumber)
{
	static_cast<void>(signalNumber);
	for (std::vector<int>::iterator it = serverSocket.begin();
		it != serverSocket.end(); ++it)
		close (*it);
	for (std::vector<struct addrinfo *>::iterator it = address.end();
		it != address.end(); ++it)
		freeaddrinfo(*it);
	// delete(configFile);
	std::cout << "\nwebserv: Exit webserver is success. :)" << std::endl;
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	int					errorNumber;
	// std::vector<int>	serverSocket;
	// std::vector<struct addrinfo *> address;
	int					clientSocket;
	int					socketFd;
	Config				*configFile;

	signal(SIGINT, signalHandle);
	// * checking the valid of argument [ Your program has to tacke a configuration file as argument, or use a default path ??] ;
	if (argc == 1)
		configFile = new Config();
	else if (argc == 2)
		configFile = new Config(argv[1]);
	else
		exitWithError((char *)"webserv: Error: wrong argument.\n(hint) ./webserv [configurtion_file]", EE_NONE);
	struct addrinfo hint;
	struct addrinfo *addrList;

	memset(&hint, '\0', sizeof(struct addrinfo));
	hint.ai_family = AF_UNSPEC; // * allow ip address AF_INET ipv4, AF_INET6 ipv6 ;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_flags = AI_PASSIVE;
	// TODO change NULL -> server_name;
	errorNumber = getaddrinfo("127.0.0.1", configFile->getServer()[0]->listen.c_str(), &hint, &addrList); // * << change position 0 to i for dynamic;
	if (errorNumber != 0)
		exitWithError(const_cast<char *>(gai_strerror(errorNumber)), EE_PERR); // TODO checkerror output;
	
	for (struct addrinfo *ptr = addrList;; ptr = ptr->ai_next)
	{
		if (ptr == NULL)
			exitWithError((char *)"webserv: ERROR: ", EE_PERR);

		// * open the socket for web server;
		socketFd = socket(ptr->ai_family, ptr->ai_socktype, 0);
		if (socketFd < 1)
			continue ;

		// * set option for socket;

		// * bind socket with value in struct sockaddr_in;
		if (bind(socketFd, ptr->ai_addr, ptr->ai_addrlen) == 0)
		{
			// fcntl(socketFd, F_SETFL, O_NONBLOCK);
			serverSocket.push_back(socketFd);
			address.push_back(ptr);
			break ;
		}
	}

	// * set the server to listen the client with number of back_log;
	if (listen(socketFd, BACK_LOG) < 0) // TODO change backlog with config value
		exitWithError((char *)"webserv: ERROR: ", EE_PERR);

	fd_set	currentSocket;
	fd_set	readySocket;
	int		maxSocketFd;

	FD_ZERO(&currentSocket);
	for (int i = 0; i < serverSocket.size(); ++i)
	{
		FD_SET(serverSocket[i], &currentSocket);
		// std::cout << i << std::endl;
	}

	maxSocketFd = serverSocket.back() + 1;
	// std::cout << "server ready to pairing" << std::endl;
	while (true)
	{
		readySocket = currentSocket;

		std::cout << "********** Waiting for Client Request **********" << std::endl;
		if (select(maxSocketFd, &readySocket, NULL, NULL, NULL) < 0)
			exitWithError((char *)"webserve: ERROR", EE_PERR);
		std::cout << "********** Client Sent Some Request **********" << std::endl;
		for (int socket = 0; socket < maxSocketFd; ++socket)
		{
			// std::cout << "hello in FD_ISSET i = " << i << std::endl;
			if (FD_ISSET(socket, &readySocket))
			{
				// std::cout << "checkFd |" << i << "| in serverSocket" << std::endl;
				int isFound = 0;
				for (int j = 0; j < serverSocket.size(); ++j)
				{
					if (serverSocket[j] == socket)
					{
						std::cout << "********** Accept Client Request ***********" << std::endl;
						clientSocket = accept(serverSocket[j], address[j]->ai_addr, &address[j]->ai_addrlen);
						// fcntl(clientSocket, F_SETFL, O_NONBLOCK);
						if (clientSocket < 0)
							exitWithError((char *)"webserv: ERROR: ", EE_PERR);
						FD_SET(clientSocket, &currentSocket);
						if (clientSocket > maxSocketFd - 1)
							maxSocketFd = clientSocket + 1;
						isFound = 1;
						std::cout << "********** Accept Client Done **********" << std::endl;
					}
					// std::cout << "hello in isFound loop | j = " << j << " | serverSocker[j] : " << serverSocket[j] << std::endl;
				}
				if (!isFound)
				{
					std::cout << "********** Sending Respond to Client **********" << std::endl;
					fcntl(socket, F_SETFL, O_NONBLOCK);
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
	return (EXIT_SUCCESS);
}

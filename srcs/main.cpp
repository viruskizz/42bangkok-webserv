#include "Webserv.hpp"
#include "ServerRespond.hpp"
#include "RequestHeader.hpp"
// #include <netdb.h>

std::vector<int>				serverSocket;
std::vector<struct addrinfo *>	address;
// Config							*configFile;

// int main(void)
// {
// 	Config conf = Config();
// 	std::cout << conf << std::endl;
// 	return 0;
// }

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

// static int	serverHandler_GetFD(std::string const &url, Config const &server)
// {
// 	int		fd;
// 	char	*fileName;
// 	int		count;
//
// 	fileName = NULL;
// 	count = 0;
// 	if (url == "/")
// 	{
// 		while (count < server.getIndex().size())
// 		{
// 			fileName = NULL;
// 			// if (ROOT[4] != '/')
// 			// 	fileName = strjoin(ROOT, "/", SJ_NONE);
// 			fileName = strjoin(ROOT, const_cast<char *>(server.getIndex()[count].c_str()), SJ_NONE);
// 			std::cout << "openFirst-> " << fileName << std::endl;
// 			fd = open(fileName, O_RDONLY);
// 			delete [] fileName;
// 			if (fd >= 3)
// 				break ;
// 			count++;
// 		}
// 	}
// 	else
// 	{
// 		fileName = strjoin(ROOT, const_cast<char *>(url.c_str()), SJ_NONE);
// 		std::cout << "openSeccond-> " << fileName << std::endl;
// 		fd = open(fileName, O_RDONLY);
// 		delete [] fileName;
// 	}
// 	return (fd);
// }



// int serverHandler(int socket, Config const &server)
// {
// 	char			*buffer;
// 	int				readByte;
// 	int				fd;
// 	char			*body = NULL;
// 	int				bodySize = 0;
// 	// ServerRespond 	serverRespond(socket, server);
// 	// write(socket, serverRespond.getRespond().c_str(), serverRespond.getRespondLength());
// 	// std::cout << serverRespond << std::endl;
// 	// close(socket);
// 	// return (0);
// 	RequestHeader	clientRequest(socket);
//
// 	// * moving into server directory
// 	// * check client request while port and file is request
// 	// * read file from client request and write into respond
//
// 	// TODO currentServer = serverFindServerWithPort(clientRequest.getPort());
// 	// TODO if (currentServer.getUrl() == "/")
// 	std::cout << "********** SEVER HANDLE **********" << std::endl;
// 	// serverHandler_GetFD(clientRequest.getUrl(), server);
// 	if (clientRequest.getUrl() == "/") // * index file that name in Config file
// 	{
// 		int	count = 0;
// 		while (count < server.getIndex().size())
// 		{
// 			char	*fileName = NULL;
// 			fileName = strjoin(ROOT, const_cast<char *>(server.getIndex()[count].c_str()), SJ_NONE);
// 			std::cout << fileName << std::endl;
// 			fd = open(fileName, O_RDONLY);
// 			delete [] fileName;
// 			if (fd >= 3)
// 				break ;
// 			count++;
// 		}
// 		if (fd < 0)
// 		{
// 			perror("webserv :ERROR"); // ? maybe need return errro number for respond header.
// 			std::cout << "errno: " << errno << std::endl;
// 			return (errno);
// 		}
//
//
// 		buffer = new char[READSIZE + 1];
// 		while (true)
// 		{
// 			readByte = read(fd, buffer, READSIZE);
// 			buffer[readByte] = '\0';
// 			if (readByte <= 0)
// 				break ;
// 			body = strjoin(body, bodySize, buffer, readByte, SJ_FFIR);
// 			bodySize += readByte;
// 		}
// 		char	*length;
// 		char	*clientRespond;
// 		int		headerSize;
// 		length = itoa(bodySize);
// 		clientRespond = strjoin(html, length, SJ_FSEC);
// 		clientRespond = strjoin(clientRespond, BREAK_LINE, SJ_FFIR);
// 		clientRespond = strjoin(clientRespond, BREAK_LINE, SJ_FFIR);
// 		headerSize = strlen(clientRespond);
// 		clientRespond = strjoin(clientRespond, strlen(clientRespond), body, bodySize, SJ_BOTH);
// 		write(socket, clientRespond, headerSize + bodySize);
// 		// std::cout << clientRespond << std::endl;
// 		close(fd);
// 		close(socket);
// 		delete [] clientRespond;
// 		std::cout << "first condition..." << std::endl;
// 	}
// 	else
// 	{
// 		char	*fileName;
//
// 		fileName = strjoin(ROOT, const_cast<char *>(clientRequest.getUrl().c_str()), SJ_NONE);
// 		std::cout << fileName << std::endl;
// 		fd = open(fileName, O_RDONLY);
// 		if (fd < 0)
// 		{
// 			perror("webserv :ERROR"); // ? maybe need return errro number for respond header.
// 			std::cout << "errno: " << errno << std::endl;
// 			return (errno);
// 		}
//
// 		buffer = new char[READSIZE + 1];
// 		while (true)
// 		{
// 			readByte = read(fd, buffer, READSIZE);
// 			if (readByte <= 0)
// 				break ;
// 			buffer[readByte] = '\0';
// 			body = strjoin(body, bodySize, buffer, readByte, SJ_FFIR);
// 			bodySize += readByte;
// 		}
//
// 		std::cout << std::endl;
// 		char	*length;
// 		char	*clientRespond;
// 		int		headerSize;
//
// 		length = itoa(bodySize);
// 		if (clientRequest.getUrl().find(".png") != std::string::npos)
// 			clientRespond = strjoin(png, length, SJ_FSEC);
// 		else
// 			clientRespond = strjoin(html, length, SJ_FSEC);
// 		clientRespond = strjoin(clientRespond, BREAK_LINE, SJ_FFIR);
// 		clientRespond = strjoin(clientRespond, BREAK_LINE, SJ_FFIR);
// 		headerSize = strlen(clientRespond) + bodySize;
// 		clientRespond = strjoin(clientRespond, strlen(clientRespond), body, bodySize, SJ_BOTH);
// 		write(socket, clientRespond, headerSize);
// 		std::cout << "seccond condition..." << std::endl;
// 		delete [] clientRespond;
// 		close(fd);
// 		close(socket);
// 	}
// 	return (200);
// }


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

// int  main(int argc, char **argv)
// {
// 	int					errorNumber;
// 	int					clientSocket;
// 	int					socketFd;
// 	Config				configFile;
//
// 	signal(SIGINT, signalHandle);
// 	// * checking the valid of argument [ Your program has to tacke a configuration file as argument, or use a default path ??] ;
// 	// if (argc == 1)
// 	// 	configFile = new Config();
// 	// else if (argc == 2)
// 	// 	configFile = new Config(argv[1]);
// 	// else
// 	// 	exitWithError((char *)"webserv: Error: wrong argument.\n(hint) ./webserv [configurtion_file]", EE_NONE);
//
// 	struct addrinfo hint;
// 	struct addrinfo *addrList;
//
// 	memset(&hint, '\0', sizeof(struct addrinfo));
// 	hint.ai_family = AF_UNSPEC; // * allow ip address AF_INET ipv4, AF_INET6 ipv6 ;
// 	hint.ai_socktype = SOCK_STREAM;
// 	hint.ai_flags = AI_PASSIVE;
// 	errorNumber = getaddrinfo(NULL, configFile.getServer()[0]->listen, &hint, &addrList); // * << change position 0 to i for dynamic;
// 	if (errorNumber != 0)
// 		exitWithError(const_cast<char *>(gai_strerror(errorNumber)), EE_PERR); // TODO checkerror output;
//
// 	for (struct addrinfo *ptr = addrList;; ptr = ptr->ai_next)
// 	{
// 		if (ptr == NULL)
// 			exitWithError((char *)"webserv: ERROR: ", EE_PERR);
//
// 		// * open the socket for web server;
// 		socketFd = socket(ptr->ai_family, ptr->ai_socktype, 0);
// 		if (socketFd < 1)
// 			continue ;
//
// 		// * set option for socket;
//
// 		// * bind socket with value in struct sockaddr_in;
// 		if (bind(socketFd, ptr->ai_addr, ptr->ai_addrlen) == 0)
// 		{
// 			serverSocket.push_back(socketFd);
// 			address.push_back(ptr);
// 			break ;
// 		}
// 	}
// 	freeaddrinfo(addrList);
//
// 	// * set the server to listen the client with number of back_log;
// 	if (listen(socketFd, BACK_LOG) < 0) // TODO change backlog with config value
// 		exitWithError((char *)"webserv: ERROR: ", EE_PERR);
//
// 	fd_set	currentSocket;
// 	fd_set	readySocket;
// 	int		maxSocketFd;
//
// 	FD_ZERO(&currentSocket);
// 	for (int i = 0; i < serverSocket.size(); ++i)
// 	{
// 		FD_SET(serverSocket[i], &currentSocket);
// 		std::cout << i << std::endl;
// 	}
//
// 	maxSocketFd = serverSocket.back() + 1;
//
// 	while(1)
// 	{
// 		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
// 		socketFd = accept(serverSocket[0], address[0]->ai_addr, &address[0]->ai_addrlen);
// 		if (socketFd < 0)
// 		{
// 			perror("In accept");
// 			exit(EXIT_FAILURE);
// 		}
// 		serverHandler(socketFd, configFile);
// 		printf("------------------Hello message sent-------------------\n");
// 		close(socketFd);
// 	}
// 	return (0);
// }

// int main()
// {
//  	int 				server_fd, new_socket;
// 	// long 				valread;
// 	struct sockaddr_in	address;
// 	int					addrlen = sizeof(address);
//
// 	char *hello = (char *)"HTTP/1.1 200 OK\n"
// 							"Content-Type: text/plain\n"
// 							"Content-Length: 12\n"
// 							"\nHello world!";
//
// 	char *website = (char *)"HTTP/1.1 200 OK\r\n"
// 							"Content-Type: text/html\r\n"
// 							"Content-Length: ";
//
// 	char *pic = (char *)"HTTP/1.1 200 OK\n"
// 							// "Accept-Ranges: bytes\n"
// 							"Content-Type: text/css\r\n"
// 							"Content-Length: ";
//
// 	// Creating socket file descriptor
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("In socket");
// 		exit(EXIT_FAILURE);
// 	}
//
//
// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons( PORT );
//
// 	memset(address.sin_zero, '\0', sizeof address.sin_zero);
//
//
// 	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
// 	{
// 		perror("In bind");
// 		exit(EXIT_FAILURE);
// 	}
// 	if (listen(server_fd, 10) < 0)
// 	{
// 		perror("In listen");
// 		exit(EXIT_FAILURE);
// 	}
// 	while(1)
// 	{
// 		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
// 		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
// 		{
// 			perror("In accept");
// 			exit(EXIT_FAILURE);
// 		}
//
// 		Header test(new_socket);
// 		// std::cout << test;
//
// 		// std::cout << "-> " << new_socket << std::endl;
// 		char		buffer[1000000];
// 		int			fd, readByte, count = 0;
// 		char		*num, *respond;
// 		char		*picture = NULL;
// 		std::string	number;
//
// 		(void)hello;
// 		for (int i = 0; i < 30000; ++i)
// 			buffer[i] = '\0';
//
// 		num = (char *)calloc(20, 1);
// 		if (test.getUrl() == "/")
// 		{
// 			// char url[100];
// 			// int	i = -1;
// 			// while (test.getUrl()[++i])
// 			// 	url[i] = test.getUrl()[i];
// 			// url[i] = '\0';
// 			// std::cout << "check file name:" << url << std::endl;
// 			fd = open("./html/index.html", O_RDONLY);
// 			if (fd <= 0)
// 				std::cout << "file cannot open 1" << std::endl;
// 			readByte = read(fd, buffer, 30000);
// 			count += readByte;
// 			buffer[readByte] = '\0';
// 			number = std::to_string(readByte);
// 			int i = -1;
// 			while (number[++i])
// 				num[i] = number[i];
// 			num[i] = '\0';
// 			respond = strjoin(website, num);
// 		}
// 		else
// 		{
// 			char	url[100];
// 			for (int i = 0; i < 100; ++i)
// 				url[i] = '\0';
// 			for (int i = 0; test.getUrl()[i]; ++i)
// 				url[i] = test.getUrl()[i];
// 			fd = open(strjoin(ROOT, url), O_RDONLY);
// 			if (fd <= 0)
// 			{
// 				std::cout << "file cannot open 2" << std::endl;
// 				continue ;
// 			}
// 			while (1)
// 			{
// 				readByte = read(fd, buffer, 100000);
// 				if (readByte <= 0)
// 					break ;
// 				count += readByte;
// 			}
// 			close(fd);
// 			fd = open(strjoin(ROOT, url), O_RDONLY);
// 			picture = (char *)calloc(count, 1);
// 			while (1)
// 			{
// 				int index = 0;
// 				readByte = read(fd, buffer, 100000);
// 				buffer[readByte] = '\0';
// 				if (readByte <= 0)
// 					break ;
// 				for (int i = 0; i < readByte; ++i)
// 					picture[index++] = buffer[i];
// 				// std::cout << "|" << buffer << "|" << picture[count] << "|" << count << "|" << (int)buffer[0]<< std::endl;
// 			}
// 			// picture[0] = -119;
// 			// std::cout << picture << std::endl;
// 			for (int i = 0; i < count; ++i)
// 				std::cout << picture[i];
// 			std::cout << "-(count)-> " << count << std::endl; 
// 			std::cout << "-(picture)-> " << count << std::endl;
// 			number = std::to_string(count);
// 			int i = -1;
// 			while (number[++i])
// 				num[i] = number[i];
// 			num[i] = '\0';
// 			respond = strjoin(pic, num);
// 		}
// 		// int ii = strlen(respond);
// 		respond = strjoin(respond, (char *)"\r\n\r\n");
// 		int index = 0;
// 		if (test.getUrl() == "/")
// 		{
// 			respond = strjoin(respond, buffer);
// 			index = strlen(respond);
// 		}
// 		else
// 		{
// 			std::cout << "hello1" << std::endl;
// 			char *tmp = respond;
// 			respond = (char *)calloc(strlen(tmp) + count + 1, 1);
// 			while (tmp[index])
// 			{
// 				respond[index] = tmp[index];
// 				index++;
// 			}
// 			int i = 0;
// 			while (i < count)
// 				respond[index++] = picture[i++];
// 			i = 0;
// 			while (i < index)
// 				std::cout << respond[i++];
// 			std::cout << std::endl;
// 			std::cout << "hello2" << std::endl;
// 			// char *tmp = (char *)calloc(strlen(respond) + count + 1, 1);
// 			// for (int i = 0; respond[i]; i++)
// 				// tmp[i] = respond[i];
// 			// int index = strlen(respond);
// 			// for (int i = 0; i < count; i++)
// 			// 	memset(tmp + index + i, picture[i], 1);
// 			// respond = tmp;
// 			// // respond = strjoin(respond, picture);
// 			// std::cout << respond[ii + 605] << std::endl;
// 		}
// 		// std::cout << respond << std::endl;
// 		write(new_socket , respond, index);
// 		printf("------------------Hello message sent-------------------\n");
// 		// std::cout << buffer << std::endl;
// 		close(fd);
// 		close(new_socket);
// 	}
// 	return 0;


// // 	std::cout << "Hello main" << std::endl;
// // 	return 0;
// }

// * Cache-Control:
// * Content-Encoding:
// * Content-Length:
// * Content-Type:
// * Date:
// * Last-Modified:
// * Server:
// * Server-Timing:
// * Strict-Transport-Security:
// * X-Frame-Options:

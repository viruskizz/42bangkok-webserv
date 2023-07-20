#include "Webserv.hpp"
#include "Header.hpp"
#include <netdb.h>

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
# define ROOT (char *)"./html"
/*
* macro for exitWithError() EE_PERR with perror() | EE_NONE just string*/
# define EE_PERR 1
# define EE_NONE 0

void exitWithError(char *errorMessage, int mode)
{
	if (mode == EE_PERR)
		perror(errorMessage);
	else if (mode == EE_NONE)
		std::cerr << errorMessage << std::endl;
	exit (EXIT_FAILURE);
}

void serverHandler(void)
{

}

char	*strjoin(char *str1, char *str2)
{
	char	*result;
	int		index = 0;

	if (str1)
		result = (char*)calloc(strlen(str1) + strlen(str2) + 1, 1);
	else
		result = (char*)calloc(strlen(str2) + 1, 1);
	for (int i = 0; str1 && str1[i]; ++i, ++index)
		result[index] = str1[i];
	for (int i = 0; str2[i]; ++i, ++index)
		result[index] = str2[i];
	return (result);
}

#define BREAK_LINE "\r\n"

char *hello = (char *)"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 12\r\n"
				"\r\nHello world!";

char *html = (char *)"HTTP/1.1 200 OK\r\n"
						"Content-Type: text/plain\r\n"
						"Content-Length: ";

char *png = (char *)"HTTP/1.1 200 OK\r\n"
						"Content-Type: image/png\r\n"
						"Content-Length: ";

void	signalHandle(int signalNumber)
{
	static_cast<void>(signalNumber);
	for (std::vector<int>::iterator it = serverSocket.begin();
		it != serverSocket.end(); ++it)
		close (*it);
	for (std::vector<struct addrinfo *>::iterator it = address.end();
		it != address.end(); ++it)
		freeaddrinfo(*it);
	// std::cout << "hello" << std::endl;
	// delete(configFile);
	exit(EXIT_SUCCESS);
}



int	main(int argc, char **argv)
{
	// struct sockaddr_in	address;
	int					errorNumber;
	// std::vector<int>	serverSocket;
	// std::vector<struct addrinfo *> address;
	int					clientSocket;
	int					socketFd;
	// int					addressLength;
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
	errorNumber = getaddrinfo(NULL, PORT, &hint, &addrList);
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
			serverSocket.push_back(socketFd);
			address.push_back(ptr);
			break ;
		}
	}
	freeaddrinfo(addrList);

	// * set the server to listen the client with number of back_log;
	if (listen(socketFd, BACK_LOG) < 0) // TODO change backlog with config value
		exitWithError((char *)"webserv: ERROR: ", EE_PERR);

	// * open the socket for web server;
	// serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	// if (serverSocket < 1)
	// 	exitWithError((char *)"webserv: ERROR: ", EE_PERR);

	// * set value the sockaddr_in struct;
	// address.sin_addr.s_addr = INADDR_ANY;
	// address.sin_family = AF_INET;
	// // address.sin_port = htons(PORT);
	// memset(address.sin_zero, '\0', sizeof address.sin_zero);
	// // beZero(address.sin_zero, sizeof(address.sin_zero));
	// addressLength = sizeof(address);
	// TODO setsockopt();

	// * bind socket with value in struct sockaddr_in;
	// if (bind(serverSocket, reinterpret_cast<struct sockaddr *>(&address), addressLength) < 0)
	// 	exitWithError((char *)"webserv: ERROR: ", EE_PERR);

	// * set the server to listen the client with number of back_log;
	// if (listen(serverSocket, BACK_LOG) < 0)
	// 	exitWithError((char *)"websev: ERROR: ", EE_PERR);

	fd_set	currentSocket;
	fd_set	readySocket;
	int		maxSocketFd;

	FD_ZERO(&currentSocket);
	for (int i = 0; i < serverSocket.size(); ++i)
	{
		FD_SET(serverSocket[i], &currentSocket);
		std::cout << i << std::endl;
	}

	std::cout << FD_SETSIZE << std::endl;
	maxSocketFd = serverSocket.back() + 1;
	std::cout << "hello | " << serverSocket.back() << std::endl;
	// while (true)
	// {
	// 	readySocket = currentSocket;

	// 	std::cout << "before selected" << std::endl;
	// 	if (select(maxSocketFd, &readySocket, NULL, NULL, NULL) < 0)
	// 		exitWithError((char *)"webserve: ERROR", EE_PERR);
	// 	std::cout << "after selected" << std::endl;
	// 	for (int i = 0; i < maxSocketFd; ++i)
	// 	{
	// 		std::cout << "hello in FD_ISSET i = " << i << std::endl;
	// 		if (FD_ISSET(i, &readySocket))
	// 		{
	// 			std::cout << "checkFd |" << i << "| in serverSocket" << std::endl;
	// 			int isFound = 0;
	// 			for (int j = 0; j < serverSocket.size(); ++j)
	// 			{
	// 				if (serverSocket[j] == i)
	// 				{
	// 					std::cout << "Found fd in serverSocket" << std::endl;
	// 					clientSocket = accept(serverSocket[j], address[j]->ai_addr, &address[j]->ai_addrlen);
	// 					if (clientSocket < 0)
	// 						exitWithError((char *)"webserv: ERROR: ", EE_PERR);
	// 					FD_SET(clientSocket, &currentSocket);
	// 					if (clientSocket > maxSocketFd - 1)
	// 						maxSocketFd = clientSocket + 1;
	// 					isFound = 1;
	// 				}
	// 				std::cout << "hello in isFound loop | j = " << j << " | serverSocker[j] : " << serverSocket[j] << std::endl;
	// 			}
	// 			if (!isFound)
	// 			{
	// 				std::cout << "not Found fd in serverSocket" << std::endl;
	// 				Header	clientRequest(i);
	// 				std::cout << clientRequest << std::endl;
	// 				// * handler fucntion;
	// 				write(i, hello, strlen(hello));
	// 				close(i);
	// 				FD_CLR(i, &currentSocket);
	// 			}
	// 		}
	// 	}

		// std::cout << "********** Waiting for new connection **********" << std::endl;
		// // clientSocket = accept(serverSocket[0], reinterpret_cast<struct sockaddr *>(&address), reinterpret_cast<socklen_t *>(&addressLength));
		// clientSocket = accept(serverSocket[0], address[0]->ai_addr, &address[0]->ai_addrlen);
		// if (clientSocket < 0)
		// 	exitWithError((char *)"webserv: ERROR: ", EE_PERR);

		// Header	clientRequest(clientSocket);
		
		// std::cout << clientRequest << std::endl;

		// std::cout << "********** Server respond is sent **********" << std::endl;

		// // * handler fucntion;
		// write(clientSocket, hello, strlen(hello));
		// sleep(5);
		
		// close(clientSocket);
	// }
	for (std::vector<int>::iterator it = serverSocket.begin();
		it != serverSocket.end(); ++it)
		close (*it);
	for (std::vector<struct addrinfo *>::iterator it = address.end();
		it != address.end(); ++it)
		freeaddrinfo(*it);
	// delete(configFile);
	return (EXIT_SUCCESS);
}

// int main()
// {
//  	int 				server_fd, new_socket;
// 	// long 				valread;
// 	struct sockaddr_in	address;
// 	int					addrlen = sizeof(address);

// 	char *hello = (char *)"HTTP/1.1 200 OK\n"
// 							"Content-Type: text/plain\n"
// 							"Content-Length: 12\n"
// 							"\nHello world!";

// 	char *website = (char *)"HTTP/1.1 200 OK\r\n"
// 							"Content-Type: text/html\r\n"
// 							"Content-Length: ";

// 	char *pic = (char *)"HTTP/1.1 200 OK\n"
// 							// "Accept-Ranges: bytes\n"
// 							"Content-Type: text/css\r\n"
// 							"Content-Length: ";

// 	// Creating socket file descriptor
// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("In socket");
// 		exit(EXIT_FAILURE);
// 	}


// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons( PORT );

// 	memset(address.sin_zero, '\0', sizeof address.sin_zero);


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

// 		Header test(new_socket);
// 		// std::cout << test;

// 		// std::cout << "-> " << new_socket << std::endl;
// 		char		buffer[1000000];
// 		int			fd, readByte, count = 0;
// 		char		*num, *respond;
// 		char		*picture = NULL;
// 		std::string	number;

// 		(void)hello;
// 		for (int i = 0; i < 30000; ++i)
// 			buffer[i] = '\0';

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

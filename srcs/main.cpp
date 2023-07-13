#include "Webserv.hpp"

int main(void)
{
	Config conf = Config();
	std::cout << conf << std::endl;
	return 0;
}

// # define PORT 8080
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <cstring>
// #include <iostream>
// #include <fcntl.h>
// #include <unistd.h>
// #include "Header.hpp"
// Saved
// void	learn() {
//  	int 				server_fd, new_socket; 
// 	// long 				valread;
// 	struct sockaddr_in	address;
// 	int					addrlen = sizeof(address);

// 	char *hello = (char *)"HTTP/1.1 200 OK\n"
// 							"Content-Type: text/plain\n"
// 							"Content-Length: 12\n"
// 							"\nHello world!";

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
// 		std::cout << test;

// 		// std::cout << "-> " << new_socket << std::endl;
// 		// char buffer[30000] = {0};
// 		// valread = read( new_socket , buffer, 30000);
// 		// std::cout << "-> " << buffer << std::endl;

		
// 		// printf("%s\n",buffer );
// 		write(new_socket , hello , strlen(hello));
// 		printf("------------------Hello message sent-------------------\n");
// 		close(new_socket);
// 	}
// }
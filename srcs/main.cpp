#include "Webserv.hpp"
#include "./server/HttpRespond.hpp"
#include "./server/HttpRequest.hpp"
#include "./server/Server.hpp"

Server *server;

static void	signalHandle(int signalNumber)
{
	static_cast<void>(signalNumber);
	for (std::vector<int>::iterator it = server->getServerSockets().begin();
		it != server->getServerSockets().end(); ++it)
		close (*it);
	for (std::vector<struct addrinfo *>::iterator it = server->getAddress().end();
		it != server->getAddress().end(); ++it)
		freeaddrinfo(*it);
	// delete(configFile);
	std::cout << "\nwebserv: Exit webserver is success. :)" << std::endl;
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	Config	*configFile;

	signal(SIGINT, signalHandle);
	// * checking the valid of argument [ Your program has to tacke a configuration file as argument, or use a default path ??] ;
	if (argc == 1)
		configFile = new Config();
	else if (argc == 2)
		configFile = new Config(argv[1]);
	else
		Server::exitWithError((char *)"webserv: Error: wrong argument.\n(hint) ./webserv [configurtion_file]", EE_NONE);
	server = new Server();
	server->init();
	server->start();
	return (EXIT_SUCCESS);
}

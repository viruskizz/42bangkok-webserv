#include "Webserv.hpp"
#include "./server/HttpRespond.hpp"
#include "./server/HttpRequest.hpp"
#include "./server/Server.hpp"

Server	*server;
Config	*configFile;

static void	signalHandle(int signalNumber)
{
	size_t index;

	static_cast<void>(signalNumber);
	index = 0;
	while (index < server->getServerSockets().size())
		close (server->getServerSockets().at(index++));
	index = 0;
	while (index < server->getAddress().size())
		freeaddrinfo(server->getAddress().at(index++));
	delete configFile;
	delete server;
	std::cout << "\nwebserv: Exit webserver is success. :)" << std::endl;
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	configFile = NULL;
	server = NULL;
	signal(SIGINT, signalHandle);
	if (argc == 1)
		configFile = new Config();
	else if (argc == 2)
		configFile = new Config(argv[1]);
	else
		exitWithError((char *)"webserv: Error: wrong argument.\n(hint) ./webserv [configurtion_file]", EE_NONE);
	server = new Server();
	configFile->printConfig();
	server->init(*configFile);
	server->start(*configFile);
	return (EXIT_SUCCESS);
}

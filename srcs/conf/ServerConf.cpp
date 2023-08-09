#include "ServerConf.hpp"

ServerConf::ServerConf()
: listen(""), serverName(""), root("") {}

ServerConf::~ServerConf() {
	std::cout << "(ServerConf) destructor is called." << std::endl;
	// ! do not forget to free 
	// if (listen)
	// 	delete [] this->listen;
	// if (serverName)
	// 	delete [] this->serverName;
	// if (root)
	// 	delete [] this->root;
	std::cout << "(ServerConf) everything is freed." << std::endl;
}

std::ostream & operator << (std::ostream & o, ServerConf const & rhs) {
	o << "{";
	o << "  listen: " << rhs.listen << std::endl;
	o << "  serverName: " << rhs.serverName << std::endl;
	o << "  root: " << rhs.root << std::endl;
	o << "}";
	return o;
}
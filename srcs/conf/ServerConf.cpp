#include "ServerConf.hpp"

ServerConf::ServerConf() {}

ServerConf::~ServerConf() {}

std::ostream & operator << (std::ostream & o, ServerConf const & rhs) {
	o << "{";
	o << "  listen: " << rhs.listen << std::endl;
	o << "  serverName: " << rhs.serverName << std::endl;
	o << "  root: " << rhs.root << std::endl;
	o << "}";
	return o;
}

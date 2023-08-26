#include "ServerConf.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

ServerConf::ServerConf(Config *conf, std::ifstream & ifile) {
	m_serverName = "";
	m_listen = conf->getListen();
	m_root = conf->getRoot();
	m_index = conf->getIndex();
	// std::ifstream ifile(conf->getFilename().c_str());
	lineByLine(ifile, &ServerConf::setConfig);
}

ServerConf::~ServerConf() {
	std::cout << "(ServerConf) everything is freed." << std::endl;
}

string const & ServerConf::getListen() const {
	return m_listen;
}

string const & ServerConf::getServerName() const {
	return m_serverName;
}

string const & ServerConf::getRoot() const {
	return m_root;
}

string const & ServerConf::getIndex() const {
	return m_index;
}

/************************************************
 *           Specific member function           *
 ************************************************/

void ServerConf::setConfig(string const & key, string const & val) {
	if (key == "location") {
		// filedata += setLocation(file);
	} else {
		mapKeyVal(key, val);
	}
}

void ServerConf::setLocation(string const & key, string const & val) {
	
}

void ServerConf::lineByLine(std::ifstream & ifile, void (ServerConf::*func)(string const & key, string const & val)){
	string line;
	while (getline(ifile, line)) {
		if (!line.empty() && line.at(0) == '#') {
			continue;
		}
		vector<string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) { continue; }
		if (value.size() == 1 && value[0] == "}") {
			return;
		}
		if (value.size() < 2) { throw "Invalid Key Value string"; }
		string key = value[0];
		string val = value[1];
		(this->*func)(key, val);
	}
}

void ServerConf::mapKeyVal(string const & key, string const & val) {
	if (key == "server_name") {
		m_serverName = val;
	}
	if (key == "listen") {
		m_listen = val;
	}
	if (key == "root") {
		m_root = val;
	}
}

std::ostream & operator << (std::ostream & o, ServerConf const & rhs) {
	o << "{";
	o << "  listen: " << rhs.getServerName() << std::endl;
	o << "  serverName: " << rhs.getListen() << std::endl;
	o << "  root: " << rhs.getRoot() << std::endl;
	o << "}";
	return o;
}
#include "ServerConf.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

ServerConf::ServerConf()
: m_listen(""), m_serverName(""), m_root(""), m_locations() {}

ServerConf::ServerConf(std::ifstream & ifile): m_listen(""), m_serverName(""), m_root(""), m_locations() {
	// setConfig(ifile);
	// lineByLine(ifile, &setConfig);
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

/************************************************
 *           Specific member function           *
 ************************************************/

// string ServerConf::setConfig(std::ifstream & file) {
// 	string filedata = "";
// 	string line;
// 	ServerConf *conf = new ServerConf();
// 	while (getline(file, line)) {
// 		// if (file.peek() == EOF) { throw ServerConf::InvalidConfigException(); }
// 		filedata += line + '\n';
// 		if (!line.empty() && line.at(0) == '#') {
// 			continue;
// 		}
// 		if (!line.empty() && line == "}") {
// 			return filedata;
// 		}
// 		std::vector<string> value = StringUtil::split(line, " \t");
// 		if (value.size() == 0) { continue;; }
// 		if (value.size() < 2) { throw ServerConf::InvalidConfigException(); }
// 		string key = value[0];
// 		string val = value[1];
// 		if (key == "location") {
// 			if (val != "{") { throw ServerConf::InvalidConfigException(); }
// 			filedata += setLocation(file);
// 		} else {
// 			mapKeyVal(key, val);
// 		}
// 	}
// 	return filedata;
// }


void ServerConf::setConfig(string const & key, string const & val) {
	if (key == "location") {
		// if (val != "{") { throw ServerConf::InvalidConfigException(); }
		// filedata += setLocation(file);
	} else {
		mapKeyVal(key, val);
	}
}

void ServerConf::setLocation(string const & key, string const & val) {
	
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
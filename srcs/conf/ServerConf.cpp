#include "ServerConf.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

ServerConf::ServerConf(Config *conf, std::ifstream & ifile): m_ifile(ifile), m_serverName("") {
	m_listen = conf->getListen();
	m_root = conf->getRoot();
	m_index = conf->getIndex();
	lineByLine(m_ifile, &ServerConf::setServer);
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

vector<StringMap> const & ServerConf::getLocations(void) const {
	return m_locations;
}

/************************************************
 *           Specific member function           *
 ************************************************/

void ServerConf::setServer(string const & key, string const & val) {
	if (key == "server_name") {
		m_serverName = val;
	}
	if (key == "listen") {
		m_listen = val;
	}
	if (key == "root") {
		m_root = val;
	}
	if (key == "location" && val != "{") { throw "Invalid Key Value string"; }
	if (key == "location") {
		map<string, string> mapStr = lineByLine(m_ifile, NULL);
		m_locations.push_back(mapStrLocation(mapStr));
	}
}

StringMap ServerConf::mapStrLocation(map<string, string> mapStr) {
	map<string, string> locate;
	for(map<string, string>::const_iterator it = mapStr.begin(); it != mapStr.end(); ++it) {
		for(int i = 0; i < sizeof(ServerConf::LOCATION_KEYS) / sizeof(ServerConf::LOCATION_KEYS[0]); i++) {
			if (string(ServerConf::LOCATION_KEYS[i]) == string(it->first)) {
				locate[it->first] = it->second;
			}
		}
	}
	return locate;
}

StringMap ServerConf::lineByLine(std::ifstream & ifile, void (ServerConf::*func)(string const & key, string const & val)){
	string line;
	map<string, string> mapStr;
	while (getline(ifile, line)) {
		if (!line.empty() && line.at(0) == '#') {
			continue;
		}
		vector<string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) { continue; }
		if (value.size() == 1 && value[0] == "}") {
			return mapStr;
		}
		if (value.size() < 2) { throw "Invalid Key Value string"; }
		string key = value[0];
		string val = value[1];
		mapStr[key] = val;
		if (func) {
			(this->*func)(key, val);
		}
	}
	return mapStr;
}

const char* ServerConf::LOCATION_KEYS[] = {
	"path",
	"root"
};

std::ostream & operator << (std::ostream & o, ServerConf const & rhs) {
	o << "{";
	o << "  listen: " << rhs.getServerName() << std::endl;
	o << "  serverName: " << rhs.getListen() << std::endl;
	o << "  root: " << rhs.getRoot() << std::endl;
	o << "}";
	return o;
}
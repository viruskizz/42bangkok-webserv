#include "Config.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

Config::Config() : m_filename("./files/default.conf"), m_filedata("") {
	setFiledata();
	m_ifile.open(m_filename.c_str());
	lineByLine(m_ifile, &Config::setConfig);
}

Config::Config(std::string const & filename) : m_filename(filename), m_filedata("") {
	setFiledata();
	m_ifile.open(m_filename.c_str());
	lineByLine(m_ifile, &Config::setConfig);
}

Config::~Config() {
	std::cout << "[Config] destructor is called" << std::endl;
	for (std::vector<ServerConf*>::iterator it = m_servers.begin();
		it != m_servers.end();) {
		delete *it;
		m_servers.erase(it);
	}
	std::cout << "size: " << m_servers.size() << std::endl;
}

string const & Config::getFilename() const {
	return m_filename;
}

string const & Config::getFiledata() const {
	return m_filedata;
}

string const &Config::getListen(void) const {
	return m_listen;
}

string const & Config::getRoot() const {
	return m_root;
}

string const &Config::getIndex(void) const {
	return m_index;
}

vector<ServerConf *> const & Config::getServers(void) const {
	return m_servers;
}

/************************************************
 *           Specific member function           *
 ************************************************/

void Config::setFiledata() {
	std::string line;
	std::ifstream file(m_filename.c_str());
	if (file.is_open()) {
		while (getline(file, line)) {
			m_filedata += line + '\n';
		}
		file.close();
	}
	else {
		throw Config::FileNotFoundException();
	}
}

void Config::setConfig(std::string & key, std::string & val) {
	if (key == "index") {
		m_index = val;
	}
	if (key == "server") {
		if (val != "{") {
			throw Config::InvalidConfigException();
		}
		ServerConf *serverConf = new ServerConf(this, m_ifile);
		m_servers.push_back(serverConf);
	}
}

void Config::lineByLine(std::ifstream & ifile, void (Config::*func)(string & key, string & val)) {
	string line;
	while (getline(ifile, line)) {
		if (!line.empty() && line.at(0) == '#') { continue; }
		vector<string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) { continue; }
		std::cout << "Line: " << line << std::endl;
		if (value.size() < 2) {
			std::cout << "Error Line: " << line << std::endl;
			throw Config::InvalidConfigException();
		}
		string key = value[0];
		string val = value[1];
		(this->*func)(key, val);
	}
}

void Config::debug(Config &config) {
	std::cout << C_RED << "Debug: " << config.m_filename << C_RESET << std::endl;
	std::cout << "filename: " << config.m_filename << std::endl;
	std::cout << "index: " << config.m_index << std::endl;
	std::cout << "servers: " << config.m_servers.size() << std::endl;
	for(int i = 0; i < config.m_servers.size(); i++) {
		ServerConf *serv = config.m_servers[i];
		std::cout << "[" << std::endl;
		std::cout << "  {" << std::endl;
		std::cout << "     serverName: " << serv->getServerName() << std::endl;
		std::cout << "     listen: " << serv->getListen() << std::endl;
		std::cout << "     root: " << serv->getRoot() << std::endl;
		std::cout << "     index: " << serv->getIndex() << std::endl;
		std::cout << "  }" << std::endl;
		std::cout << "]" << std::endl;
	}
}

std::ostream & operator << (std::ostream & o, Config const & rhs) {
	return o << rhs.getFiledata();
}

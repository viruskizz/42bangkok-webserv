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
	// std::cout << "[Config] destructor is called" << std::endl;
	for (std::vector<ServerConf*>::iterator it = m_servers.begin();
		it != m_servers.end();) {
		delete *it;
		m_servers.erase(it);
	}
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
		exitWithError((char *)"webserv: Error: File not found or not enough permission to open.", EE_NONE);//throw Config::FileNotFoundException();
	}
}

void Config::setConfig(std::string & key, std::string & val) {
	if (key == "index") {
		m_index = val;
	}
	if (key == "server") {
		if (val != "{") {
			exitWithError((char *)"webserv: Error: Configuration is invalid.", EE_NONE);//throw Config::InvalidConfigException();
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
		if (value.size() < 2) {
			std::cout << "Error Line: " << line << std::endl;
			exitWithError((char *)"webserv: Error: Configuration is invalid.", EE_NONE);//throw Config::InvalidConfigException();
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
	if (config.m_servers.size() == 0) { return; }
	std::cout << "[" << std::endl;
	for(size_t i = 0; i < config.m_servers.size(); i++) {
		ServerConf *serv = config.m_servers[i];
		std::cout << StringUtil::space(2) << "{" << std::endl;
		std::cout << StringUtil::space(4) << "serverName: " << serv->getServerName() << std::endl;
		std::cout << StringUtil::space(4) << "listen: " << serv->getListen() << std::endl;
		std::cout << StringUtil::space(4) << "root: " << serv->getRoot() << std::endl;
		std::cout << StringUtil::space(4) << "index: " << serv->getIndex() << std::endl;
		vector<StringMap> locations = serv->getLocations();
		if (locations.size() > 0) {
			std::cout << StringUtil::space(4) << "locations: [" << std::endl;
			for (vector<StringMap>::iterator itLocate = locations.begin(); itLocate < locations.end(); itLocate++) {
				StringMap mapLocate = *itLocate;
				std::cout << StringUtil::space(6) << "{" << std::endl;
				for(map<string, string>::const_iterator it = mapLocate.begin(); it != mapLocate.end(); ++it) {
					std::cout << StringUtil::space(8);
					std::cout << it->first << ": " << it->second << std::endl;
				}
				std::cout << StringUtil::space(6) << ((itLocate + 1 < locations.end()) ? "}," : "}") << std::endl;
			}
			std::cout << StringUtil::space(4) << "]" << std::endl;
		}
		std::cout << StringUtil::space(2) << ((i < config.m_servers.size() - 1) ? "}," : "}") << std::endl;
	}
	std::cout << "]" << std::endl;
}

void Config::printConfig(void) const
{
	std::cout << "================= severSize: [" << this->m_servers.size()  << "] =================" << std::endl;
	for (size_t i = 0; i < this->m_servers.size(); ++i)
	{
		std::cout << "• server        : [" << i << "]" << std::endl;
		std::cout << "• serverName    : " << this->m_servers.at(i)->getServerName() << std::endl;
		std::cout << "• listen        : " << this->m_servers.at(i)->getListen() << std::endl;
		std::cout << "• root          : " << this->m_servers.at(i)->getRoot() << std::endl;
		std::cout << "• index         : " << this->m_servers.at(i)->getIndex() << std::endl;
		std::cout << "• directoryList : " << this->m_servers.at(i)->getDirList() << std::endl;
		std::cout << "• clientSize    : " << this->m_servers.at(i)->getClientSize() << std::endl;
		std::cout << "• method        : ";
		for (vector<string>::const_iterator it = this->m_servers.at(i)->getMethod().begin();
			it != this->m_servers.at(i)->getMethod().end(); ++it)
			std::cout << *it << ",";
		std::cout << "\n• returnPageSize: " << this->m_servers.at(i)->getReturnPage().size() << std::endl;
		if (this->m_servers.at(i)->getReturnPage().size())
			std::cout << "------------ return ------------" << std::endl;
		for (map<string, string>::const_iterator it = this->m_servers.at(i)->getReturnPage().begin();
			this->m_servers.at(i)->getReturnPage().size() && it != this->m_servers.at(i)->getReturnPage().end(); ++it)
			std::cout << "-> " << it->first << " " << it->second << std::endl;
		if (this->m_servers.at(i)->getReturnPage().size())
			std::cout << "--------------------------------" << std::endl;
		std::cout << "• locationSize  : " << this->m_servers.at(i)->getLocations().size() << std::endl;
 		for (vector<StringMap>::const_iterator it = this->m_servers.at(i)->getLocations().begin();
			it != this->m_servers.at(i)->getLocations().end(); ++it)
		{
			std::cout << "----------- location -----------" << std::endl;
			for (map<string, string>::const_iterator iter = it->begin(); iter != it->end(); ++iter)
				std::cout << "-> " << iter->first << ": " << iter->second << std::endl;
		}
		std::cout << "===================== end =======================" << std::endl;
	}
}

std::ostream & operator << (std::ostream & o, Config const & rhs) {
	return o << rhs.getFiledata();
}

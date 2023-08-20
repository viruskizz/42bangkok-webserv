#include "Config.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

Config::Config() : m_filename("./files/default.conf"), m_filedata("") {
	// this->readfile();
	setFiledata();
}

Config::Config(std::string const & filename) : m_filename(filename), m_filedata("") {
	// this->readfile();
	setFiledata();
}

Config::~Config() {
	std::cout << "[Config] destructor is called" << std::endl;
	for (std::vector<ServerConf*>::iterator it = this->m_servers.begin();
		it != this->m_servers.end();)
	{
		delete *it;
		this->m_servers.erase(it);
	}
	std::cout << "size: " << this->m_servers.size() << std::endl;
}

std::string const & Config::getFilename() const
{
	return this->m_filename;
}

std::string const & Config::getFiledata() const
{
	return this->m_filedata;
}



std::string const	&Config::getIndex(void) const
{
	return (this->m_index);
}

std::vector<ServerConf *> const	&Config::getServers(void) const
{
	return (this->m_servers);
}

/************************************************
 *           Specific member function           *
 ************************************************/

void Config::readfile()
{
	std::string line;
	std::ifstream file(m_filename.c_str());

	if (file.is_open())
	{
		while (getline(file, line)) {
			m_filedata += line + '\n';
			if (!line.empty() && line.at(0) == '#') {
				continue;
			}
			if (!line.empty()) {
				this->setConfig(line, file);
			}
		}
		file.close();
	}
	else
	{
		throw Config::FileNotFoundException();
	}
}


void Config::setFiledata() {
	std::string line;
	std::ifstream file(m_filename.c_str());
	if (file.is_open())
	{
		while (getline(file, line)) {
			m_filedata += line + '\n';
		}
		file.close();
	}
	else
	{
		throw Config::FileNotFoundException();
	}
}

void Config::setConfig(std::string line, std::ifstream & file) {
	std::vector<std::string> value = StringUtil::split(line, " \t");
	if (value.size() == 0) { return; }
	if (value.size() < 2) { throw Config::InvalidConfigException(); }
	std::string key = value[0];
	std::string val = value[1];
	if (key == "index") {
		m_index = val;
	}
	if (key == "server") {
		if (val != "{") {
			throw Config::InvalidConfigException();
		}
		ServerConf *serverConf = new ServerConf();
	}
}


void Config::lineByLine(std::ifstream & ifile, void (*func)(string const &, string const &)) {
	string line;
	while (getline(ifile, line)) {
		if (!line.empty() && line.at(0) == '#') {
			continue;
		}
		vector<string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) { continue;; }
		if (value.size() < 2) { throw "Invalid Key Value string"; }
		string key = value[0];
		string val = value[1];
		func(key, val);
	}
}


void Config::debug(Config &config) {
	std::cout << C_RED << "Debug: " << config.m_filename << C_RESET << std::endl;
	std::cout << config.m_filedata << std::endl;

}

std::ostream & operator << (std::ostream & o, Config const & rhs) {
	return o << rhs.getFiledata();
}

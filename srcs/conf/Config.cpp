#include "Config.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

Config::Config() : m_filename("./files/default.conf"), m_filedata("") {
	this->readfile();
}

Config::Config(std::string const & filename) : m_filename(filename), m_filedata("") {
	this->readfile();
}

Config::~Config() {
	std::cout << "[Config] destructor is called" << std::endl;
	// for (std::vector<ServerConf*>::iterator it = this->m_servers.begin();
	// 	it != this->m_servers.end();)
	// {
	// 	delete *it;
	// 	this->m_servers.erase(it);
	// }
	std::cout << "size: " << this->m_servers.size() << std::endl;
}

std::string Config::getFilename() const
{
	return this->m_filename;
}

std::string Config::getFiledata() const
{
	return this->m_filedata;
}

std::vector<ServerConf *> const	&Config::getServer(void) const
{
	return (this->m_servers);
}

std::vector<std::string> const	&Config::getIndex(void) const
{
	return (this->m_index);
}

/************************************************
 *           Specific member function           *
 ************************************************/
static char	*stringDupTosCharPtr(std::string const &string);

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

void Config::setConfig(std::string line, std::ifstream & file) {
	std::vector<std::string> value = StringUtil::split(line, " \t");
	if (value.size() == 0) {
		return;
	}
	std::string key = value[0];
	value.erase(value.begin());
	if (value.size() == 0) {
		throw Config::InvalidConfigException();
	}
	if (key == "index") {
		// m_index = value.at(0);
		for (std::vector<std::string>::iterator it = value.begin(); it != value.end(); ++it)
			m_index.push_back(*it);
			// m_index.push_back(value.at(0));
	}
	if (key == "server") {
		if (value.at(0) != "{") {
			throw Config::InvalidConfigException();
		}
		setServerConf(line, file);
	}
}

void Config::setServerConf(std::string line, std::ifstream & file) {
	ServerConf *conf = new ServerConf();
	while (getline(file, line)) {
		m_filedata += line + '\n';
		if (!line.empty() && line.at(0) == '#') {
			continue;
		}
		if (!line.empty() && line == "}") {
			this->m_servers.push_back(conf);
			return;
		}
		std::vector<std::string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) {
			this->m_servers.push_back(conf);
			return;
		}
		std::string key = value[0];
		value.erase(value.begin());
		if (value.size() == 0) {
			throw Config::InvalidConfigException();
		}
		if (key == "server_name") {
			conf->serverName = value[0];
			// conf->serverName = stringDupTosCharPtr(value[0]);
			// conf.serverName = const_cast<char *>(value[0].c_str());
		}
		if (key == "listen") {
			conf->listen = value[0];
			// conf->listen = stringDupTosCharPtr(value[0]);
			// conf.listen = const_cast<char *>(value[0].c_str());
			// std::stringstream ss;
			// ss << value[0];
			// ss >> conf.listen;
		}
		if (key == "root") {
			conf->root = value[0];
			// conf->root = stringDupTosCharPtr(value[0]);
			// conf.root = const_cast<char *>(value[0].c_str());
		}
	}
	throw Config::InvalidConfigException();
}

std::ostream & operator << (std::ostream & o, Config const & rhs) {
	return o << rhs.getFiledata();
}

// static char	*stringDupTosCharPtr(std::string const &string)
// {
// 	char	*result;
// 	int		index;

// 	index = 0;
// 	result = new char[string.size() + 1];
// 	while (index < string.size())
// 	{
// 		result[index] = string[index];
// 		index++;
// 	}
// 	result[index] = '\0';
// 	return (result);
// }

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

Config::~Config() {}

std::string Config::getFilename() const
{
	return this->m_filename;
}

std::string Config::getFiledata() const
{
	return this->m_filedata;
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

void Config::setConfig(std::string line, std::ifstream & file) {
	std::vector<std::string> value = StringUtil::split(line, " \t");
	if (value.size() == 0) {
		return;
	}
	std::string key = value[0];
	value.erase(value.begin());
	if (key == "index") {
		if (value.size() == 0) {
			throw Config::InvalidConfigException();
		}
		m_index = value.at(0);
	}
	if (key == "server") {
		if (value.size() == 0) {
			throw Config::InvalidConfigException();
		}
		if (value.at(0) != "{") {
			throw Config::InvalidConfigException();
		}
		setServerConf(line, file);
	}
}

bool Config::setServerConf(std::string line, std::ifstream & file) {
	ServerConf conf = ServerConf();
	while (getline(file, line)) {
		m_filedata += line + '\n';
		if (!line.empty() && line.at(0) == '#') {
			continue;
		}
		if (!line.empty() && line == "}") {
			return;
		}
		std::vector<std::string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) {
			return;
		}
		std::string key = value[0];
		value.erase(value.begin());
		if (key == "index") {
			if (value.size() == 0) {
				throw Config::InvalidConfigException();
			}
			m_index = value.at(0);
		}
	}
	throw Config::InvalidConfigException();
}

std::ostream & operator << (std::ostream & o, Config const & rhs) {
	return o << rhs.getFiledata();
}

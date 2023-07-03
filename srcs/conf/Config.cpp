#include "Config.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

Config::Config() : m_filename("./files/default.conf"), m_filedata("") {
	this->readfile();
}

Config::Config(std::string filename) : m_filename(filename), m_filedata("") {
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
		}
		file.close();
	}
	else
	{
		throw Config::FileNotFoundException();
	}
}

void setConfig(std::string line) {
	// std
}

std::ostream & operator << (std::ostream & o, Config const & rhs) {
	return o << rhs.getFiledata();
}

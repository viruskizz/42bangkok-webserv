#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ServerConf.hpp"
#include "../utils/StringUtil.hpp"

class Config
{
private:

	void	readfile(void);
	void	setConfig(std::string, std::ifstream &);
	bool	setServerConf(std::string, std::ifstream &);

	std::string 			m_filename;
	std::string				m_filedata;
	std::string				m_index;
	std::vector<ServerConf> m_servers;

public:
	Config(void);
	Config(std::string const &);
	~Config(void);

	std::string getFiledata(void) const;
	std::string getFilename(void) const;

	class FileNotFoundException: public std::exception {
		public:
			virtual const char* what() const throw() { return "File not found or not enough permission to open"; }
	};
	class InvalidConfigException: public std::exception {
		public:
			virtual const char* what() const throw() { return "Configuration is invalid"; }
	};
};

std::ostream & operator << (std::ostream &, Config const &);
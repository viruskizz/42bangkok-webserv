#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ServerConf.hpp"

class Config
{
private:

	void	readfile(void);

	std::string m_filename;
	std::string m_filedata;
	std::vector<ServerConf> servers;

public:
	Config(void);
	Config(std::string file);
	~Config();

	std::string getFiledata(void) const;
	std::string getFilename(void) const;

	class FileNotFoundException: public std::exception {
		public:
			virtual const char* what() const throw() { return "File not found or not enough permission to open"; }
	};
};

std::ostream & operator << (std::ostream &, Config const &);
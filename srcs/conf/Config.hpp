#pragma once
# include "../Webserv.hpp"
# include "ServerConf.hpp"

class ServerConf;

class Config
{
private:

	void	readfile(void);
	void	setFiledata(void);
	void	setConfig(std::string, std::ifstream &);

	std::string 				m_filename;
	std::string					m_filedata;
	std::string					m_index;
	std::vector<ServerConf *>	m_servers;

protected:
	void lineByLine(std::ifstream & ifile, void (*func)(string const &, string const &));

public:
	Config(void);
	Config(std::string const &);
	~Config(void);

	static void debug(Config &);

	std::string const & getFiledata(void) const;
	std::string const & getFilename(void) const;

	// * sharnvon added
	std::vector<ServerConf*> const	&getServers(void) const;
	std::string const	&getIndex(void) const;

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
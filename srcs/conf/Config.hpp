#pragma once
# include "../Webserv.hpp"
# include "ServerConf.hpp"

using std::string;
using std::vector;
using std::map;

typedef map<string, string> StringMap;
// typedef StringUtil SUtil;

class ServerConf;

class Config
{
private:

	void	setFiledata(void);
	void	setConfig(string &, string &);
	

	string m_filename;
	string m_filedata;
	std::ifstream m_ifile;

	string m_index;
	string m_listen;
	string m_root;
	vector<ServerConf *> m_servers;

protected:
	void lineByLine(std::ifstream & ifile, void (Config::*func)(string &, string &));

public:
	Config(void);
	Config(string const &);
	~Config(void);

	static void debug(Config &);

	string const & getFiledata(void) const;
	string const & getFilename(void) const;
	string const & getListen(void) const;
	string const & getRoot(void) const;
	string const & getIndex(void) const;
	vector<ServerConf*> const &getServers(void) const;

	void printConfig(void) const;
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
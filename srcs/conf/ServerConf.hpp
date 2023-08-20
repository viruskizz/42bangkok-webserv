#pragma once
# include "../Webserv.hpp"
# include "./Config.hpp"

using std::string;
using std::vector;
using std::map;

enum LocationConf {
	path_pattern,
	proxy_path,
	fastcgi,
};

class ServerConf
{
private:
	/* data */
	string	m_listen;
	string	m_serverName;
	string	m_root;
	vector< map<LocationConf, string> > m_locations;

	void mapKeyVal(string const &, string const &);
	void setConfig(string const &, string const &);
	void setLocation(string const &, string const &);

public:
	ServerConf(void);
	ServerConf(std::ifstream &);
	~ServerConf(void);

	string const & getServerName(void) const;
	string const & getListen(void) const;
	string const & getRoot(void) const;


	// string setConfig(std::ifstream &);
	class InvalidConfigException: public std::exception {
		public:
			virtual const char* what() const throw() { return "Server Configuration is invalid"; }
	};
	class InvalidDuplicatedException: public std::exception {
		public:
			virtual const char* what() const throw() { return "Duplicated server config key"; }
	};
};
// string const & lineByline(std::ifstream & ifile, string const & (*func)(void));
std::ostream & operator << (std::ostream &, ServerConf const &);

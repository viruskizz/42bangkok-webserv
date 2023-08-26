#pragma once
# include "../Webserv.hpp"
# include "./Config.hpp"

class Config;

class ServerConf
{
private:
	std::ifstream & m_ifile;
	string m_serverName;
	string m_listen;
	string m_root;
	string m_index;
	vector<StringMap> m_locations;

	void setServer(string const &, string const &);
	StringMap mapStrLocation(StringMap);
	StringMap lineByLine(std::ifstream &, void (ServerConf::*)(string const &, string const &));

public:
	ServerConf(Config *, std::ifstream &);
	~ServerConf(void);

	static const char* LOCATION_KEYS[2];

	string const & getServerName(void) const;
	string const & getListen(void) const;
	string const & getRoot(void) const;
	string const & getIndex(void) const;
	vector<StringMap> const & getLocations(void) const;

	class InvalidConfigException: public std::exception {
		public:
			virtual const char* what() const throw() { return "Server Configuration is invalid"; }
	};
	class InvalidDuplicatedException: public std::exception {
		public:
			virtual const char* what() const throw() { return "Duplicated server config key"; }
	};
};

std::ostream & operator << (std::ostream &, ServerConf const &);

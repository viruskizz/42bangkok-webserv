#pragma once
# include "../Webserv.hpp"
# include "./Config.hpp"

using std::string;
using std::vector;
using std::map;

typedef map<string, string> StringMap;

class Config;

class ServerConf
{
private:
	std::ifstream & m_ifile;
	string m_serverName;
	string m_listen;
	string m_root;
	string m_index;
	string m_cgi;
	bool m_dirList;
	int m_clientSize;
	// struct timeval m_timeOut;
	vector<string> m_method;
	map<string, string> m_returnPage;
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
	vector<string> const & getMethod(void) const;
	vector<StringMap> const & getLocations(void) const;

	bool const getDirList(void) const;
	int const getClientSize(void) const;
	map<string, string> const & getReturnPage(void) const;
	string const & getCGI(void) const;

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

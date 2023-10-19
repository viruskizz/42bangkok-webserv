#include "ServerConf.hpp"

/************************************************
 * Setup default construct and  member function *
 ************************************************/

ServerConf::ServerConf(Config *conf, std::ifstream & ifile): m_ifile(ifile), m_serverName("") {
	m_listen = conf->getListen();
	m_root = conf->getRoot();
	m_index = conf->getIndex();
	m_method = split("GET,POST,DELETE", ',');
	m_dirList = true;
	m_clientSize = -1;
	lineByLine(m_ifile, &ServerConf::setServer);
	this->validate();
}

void	ServerConf::validate(void) const
{
	vector<StringMap>::const_iterator	location;
	StringMap::const_iterator			locationMap;
	bool								isFound;

	std::cout << "[Debug][Begin] validate ()" << std::endl;
	if (this->m_serverName.empty())
		throw "webserv : config-ERROR: The server-config need server name.";
	if (this->m_listen.empty())
		throw "webserv : config-ERROR: The server need listen port value.";
	else
	{
		for (int i = 0; i < this->m_listen.length(); ++i)
		{
			if (!isdigit(this->m_listen.at(i)))
				throw "webserv : config-ERROR: Invalid listen port value.";
		}
	}
	if (this->m_root.empty())
		throw "webserv : config-ERROR: the server need Root_Directory.";
	if (this->m_index.empty())
		throw "webserv : config-ERROR: the server need Index page.";
	if (!this->m_locations.empty())
	{
		location = this->m_locations.begin();
		while (location != this->m_locations.end())
		{
			if (location->find("path") != location->end() && location->find("root") != location->end()
				|| location->find("cgi_file") != location->end() && location->find("cgi_pass") != location->end())
			{
				if (location->size() < 2 && location->size() > 4)
				{
					std::cout << "[Debug] locationSize: " << location->size() << std::endl; 
					throw "webserv : config-ERROR: Invalid size of Location.";
				}
				locationMap = location->begin();
				while(locationMap != location->end())
				{
					isFound = false;
					for (int i = 0; LOCATION_KEYS[i]; ++i)
					{
						if (!strncmp(locationMap->first.c_str(), LOCATION_KEYS[i], locationMap->first.length()))
						{
							isFound = true;
							std::cout << "[Debug] " << locationMap->first << std::endl;
							break ;
						}
					}
					if (!isFound)
						throw "webserv : config-ERROR: Invalid key of Location.";
					++locationMap;
				}
			}
			else
				throw "webserv : config-ERROR: Invalid Key of Location.";
			++location;
		}
	}
	std::cout << "[Debug][End] validate ()" << std::endl;
}

ServerConf::~ServerConf() {
	std::cout << "(ServerConf) everything is freed." << std::endl;
}

string const & ServerConf::getListen() const {
	return m_listen;
}

string const & ServerConf::getServerName() const {
	return m_serverName;
}

string const & ServerConf::getRoot() const {
	return m_root;
}

string const & ServerConf::getIndex() const {
	return m_index;
}

vector<string> const & ServerConf::getMethod() const {
	return m_method;
}

vector<StringMap> const & ServerConf::getLocations(void) const {
	return m_locations;
}

bool const ServerConf::getDirList(void) const
{
	return m_dirList;
}

int const ServerConf::getClientSize(void) const
{
	return m_clientSize;
}

map<string, string> const & ServerConf::getReturnPage(void) const
{
	return m_returnPage;
}

// string const & ServerConf::getCGI(void) const
// {
// 	return m_cgi;
// }

/************************************************
 *           Specific member function           *
 ************************************************/
void ServerConf::setServer(string const & key, string const & val) {
	if (key == "server_name") {
		m_serverName = val;
	}
	else if (key == "listen") {
		m_listen = val;
	}
	else if (key == "root") {
		m_root = val;
	}
	else if (key == "method_allow"){
		this->m_method.clear();
		this->m_method = split(val, ',');
		for (vector<string>::iterator it = this->m_method.begin(); it != this->m_method.end(); ++it)
		{
			for (int i = 0; i < it->length(); ++i)
				it->at(i) = toupper(it->at(i));
			if (!strcmp(it->c_str(), "GET") && !strcmp(it->c_str(), "POST") && !strcmp(it->c_str(), "DELETE"))
				throw "webserv: config-ERROR: Invalid Medthod value.";
		}
	}
	else if (key == "directory_list")
	{
		if (val != "enable" && val != "disable")
			throw "webserv: config-ERROR: Invalid Directory_List value.";
		else if (val == "disable")
			m_dirList = false;
	}
	else if (key == "return")
	{
		vector<string>	splited = split(val, ':');
		for (int i = 0; i < splited.at(0).length(); ++i)
		{
			if (!isdigit(splited.at(0).at(i)))
			// if (!isnumber(splited.at(0).at(i)))
				throw "webserv: config-ERROR: Invalid Return values.";
		}
		if (splited.size() == 2)
			m_returnPage.insert(std::pair<string, string>(splited.at(0), splited.at(1)));
		else
			throw "webserv: config-ERROR: Invalid Return syntex.";
	}
	else if (key == "client_max_size")
	{
		for (int i = 0; i < val.size(); ++i)
		{
			if (!isdigit(val.at(i)))
				throw "webserv: config-ERROR: Invalid Client_Size value.";
		}
		m_clientSize = stringToint(val);
	}
	// * [MARK] set time out from congif
	// if (key == "timeout")
	// {
	// 	bool precision = false;
	// 	for (int i = 0; i < val.size(); ++i)
	// 	{
	// 		if (!precision && val.at(i) == '.' && i != 0 && i != val.size() - 1)
	// 			precision = true;
	// 		else if (isdigit(val.at(i)))
	// 			continue ;
	// 		else
	// 			throw "webserv: config-ERROR: Invalid Time_Out value.";
	// 	}
	// 	vector<string> splited = split(val, '.');
	// 	m_timeOut.tv_sec = stringToint(splited.at(0));
	// 	if (stringToint(splited.at(1)) > 999999)
	// 		m_timeOut.tv_usec = 999999;
	// 	else
	// 		m_timeOut.tv_usec = stringToint(splited.at(1));
	// }
	else if (key == "location" && val != "{") { throw "Invalid Key Value string"; }
	else if (key == "location") {
		map<string, string> mapStr = lineByLine(m_ifile, NULL);
		m_locations.push_back(mapStrLocation(mapStr));
	}
	else
	{
		std::cout << "[Debug] setserver () : Invalid key [" << key << "]" << std::endl;
		throw "webserv: config-ERROR: Invalid key";
	}
}

StringMap ServerConf::mapStrLocation(map<string, string> mapStr) {
	map<string, string> locate;
	for(map<string, string>::const_iterator it = mapStr.begin(); it != mapStr.end(); ++it) {
		// for(int i = 0; i < sizeof(ServerConf::LOCATION_KEYS) / sizeof(ServerConf::LOCATION_KEYS[0]); i++) {
		for(int i = 0; LOCATION_KEYS[i]; i++) {
			if (string(ServerConf::LOCATION_KEYS[i]) == string(it->first)) {
				locate[it->first] = it->second;
			}
		}
	}
	return locate;
}

StringMap ServerConf::lineByLine(std::ifstream & ifile, void (ServerConf::*func)(string const & key, string const & val)){
	string line;
	string lineTest;
	map<string, string> mapStr;
	while (getline(ifile, line)) {
		// std::cout << "[Debug] line: |" << line << "|" << std::endl;
		// line = stringTrim(line, " \t\r\n");
		// if (!line.empty() && line.at(0) == '#') {
		lineTest = stringTrim(line, " \t\r\n");
		if (!lineTest.empty() && lineTest.at(0) == '#') {
			continue;
		}
		vector<string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) { continue; }
		if (value.size() == 1 && value[0] == "}") {
			return mapStr;
		}
		if (value.size() < 2) { throw "Invalid Key Value string"; }
		string key = value[0];
		string val = value[1];
		mapStr[key] = val;
		if (func) {
			(this->*func)(key, val);
		}
	}
	return mapStr;
}

const char* ServerConf::LOCATION_KEYS[] = {
	"path",
	"root",
	"cgi_file",
	"cgi_pass",
	"client_max_size",
	"method_allow",
	NULL
};

std::ostream & operator << (std::ostream & o, ServerConf const & rhs) {
	o << "{";
	o << "  listen: " << rhs.getServerName() << std::endl;
	o << "  serverName: " << rhs.getListen() << std::endl;
	o << "  root: " << rhs.getRoot() << std::endl;
	o << "}";
	return o;
}
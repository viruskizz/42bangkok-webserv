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

	std::cout << "[Debug][Begin] validate ()" << std::endl;
	if (this->m_serverName.empty())
		throw "webserv : config-ERROR: The server-config need server name.";
	if (this->m_listen.empty())
		throw "webserv : config-ERROR: The server need listen port value.";
	else
	{
		for (size_t i = 0; i < this->m_listen.length(); ++i)
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
			locationMap = location->begin();
			while (locationMap != location->end())
			{
				if (!isStringInArray(LOCATION_KEYS, locationMap->first))
					throw "webserv : config-ERROR: Invalid Key of Location.";
				++locationMap;
			}
			if (isMapKeyFound(*location, "path") && !isMapKeyFound(*location, "root"))
				throw "webserv : config-ERROR: Key \"path\" must following with \"root\".";
			else if( isMapKeyFound(*location, "root") && !isMapKeyFound(*location, "path"))
				throw "webserv : config-ERROR: Key \"root\" must following with \"path\".";
			else if (isMapKeyFound(*location, "cgi_file") && !isMapKeyFound(*location, "cgi_pass"))
				throw "webserv : config-ERROR: Key \"cgi_file\" must following with \"cgi_pass\".";
			else if (isMapKeyFound(*location, "cgi_pass") && !isMapKeyFound(*location, "cgi_file"))
				throw "webserv : config-ERROR: Key \"cgi_pass\" must following with \"cgi_file\".";
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

bool ServerConf::getDirList(void) const
{
	return m_dirList;
}

int ServerConf::getClientSize(void) const
{
	return m_clientSize;
}

map<string, string> const & ServerConf::getReturnPage(void) const
{
	return m_returnPage;
}

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
			for (size_t i = 0; i < it->length(); ++i)
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
		for (size_t i = 0; i < splited.at(0).length(); ++i)
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
		for (size_t i = 0; i < val.size(); ++i)
		{
			if (!isdigit(val.at(i)))
				throw "webserv: config-ERROR: Invalid Client_Size value.";
		}
		m_clientSize = stringToint(val);
	}
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
		for(size_t i = 0; LOCATION_KEYS[i]; i++) {
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
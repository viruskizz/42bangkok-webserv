/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 22:43:02 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/25 05:17:10 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonGatewayInterface.hpp"
#include <fstream>

CommonGatewayInterface::CommonGatewayInterface(void) :
_protocol(""), _scheme(""), _serverPort(""), _method(""), _pathInfo(""), _pathTranslated(""),
_pathFileName(""), _scriptName(""), _fileName(""), _query(""), _clientName(""), _clientPort(""), _server(),
_cgiStatus(200)
{
	std::cout << "(CommonGatewayInterface) Defualt constructor is called." << std::endl;
}

CommonGatewayInterface::CommonGatewayInterface(Config const &server, HttpRequest const &request) : _cgiStatus(200), _server(server)
{
	std::cout << "(CommonGatewayInterface) Constructor is called." << std::endl;
	this->initScriptURI(server, request);
	this->buildEnvironment(server, request);
}

CommonGatewayInterface::CommonGatewayInterface(CommonGatewayInterface const & src)
{
	std::cout << "(CommonGatewayInterface) Copy constructor is called." << std::endl;
	*this = src;
}

CommonGatewayInterface::~CommonGatewayInterface(void)
{
	std::cout << "(CommonGatewayInterface) Destructor is called." << std::endl;
}

CommonGatewayInterface	&CommonGatewayInterface::operator=(CommonGatewayInterface const &rhs)
{
	std::cout << "(CommonGatewayInterface) Copy assiagnment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_cgiStatus = rhs._cgiStatus;
		this->_protocol = rhs._protocol;
		this->_scheme = rhs._scheme;
		this->_serverPort = rhs._serverPort;
		this->_method = rhs._method;
		this->_pathInfo = rhs._pathInfo;
		this->_pathTranslated = rhs._pathTranslated;
		this->_pathFileName = rhs._pathFileName;
		this->_scriptName = rhs._scriptName;
		this->_fileName = rhs._fileName;
		this->_query = rhs._query;
		this->_clientName = rhs._clientName;
		this->_clientPort = rhs._clientPort;
		this->_environment.clear();
		for (std::vector<std::string>::const_iterator it = rhs._environment.begin();
		it != rhs._environment.end(); ++it)
			this->_environment.push_back(*it);
	}
	return (*this);
}

int	CommonGatewayInterface::getStatusCode(void) const
{
	return (this->_cgiStatus);
}

void	CommonGatewayInterface::printValue(void) const
{
	std::cout << "=========================================================" << std::endl;
	std::cout << "==================== Class-Valiables ====================" << std::endl;
	std::cout << "=========================================================" << std::endl;
	std::cout << "• Protocol: |" << this->_protocol << "|" << std::endl;
	std::cout << "• Scheme: |" << this->_scheme << "|" << std::endl;
	std::cout << "• Server-Port: |" << this->_serverPort << "|" << std::endl;
	std::cout << "• Method: |" << this->_method << "|" << std::endl;
	std::cout << "• Path-Info: |" << this->_pathInfo << "|" << std::endl;
	std::cout << "• Path-Translated: |" << this->_pathTranslated << "|" << std::endl;
	std::cout << "• Path-Filename: |" << this->_pathFileName << "|" << std::endl;
	std::cout << "• ScripName: |" << this->_scriptName << "|" << std::endl;
	std::cout << "• FileName: |" << this->_fileName << "|" << std::endl;
	std::cout << "• Query: |" << this->_query << "|" << std::endl;
	std::cout << "• Client-Name: |" << this->_clientName << "|" << std::endl;
	std::cout << "• Client-Port: |" << this->_clientPort << "|" << std::endl;
	std::cout << "• CGI-status: |" << this->_cgiStatus << "|" << std::endl;
	std::cout << "=========================================================" << std::endl;
	std::cout << "================== Enviroment-Valiable ==================" << std::endl;
	std::cout << "=========================================================" << std::endl;
	for (std::vector<std::string>::const_iterator it = this->_environment.begin();
		it != this->_environment.end(); ++it)
		std::cout << *it << std::endl;
	std::cout << "=========================================================" << std::endl;
}

static std::string	readFile(int fd);

// TODO review ERROR with the path for executing isn't exist;
std::string	CommonGatewayInterface::executeCGI(Config const &server, HttpRequest const &request)
{
	std::string	content;
	int			fd;
	char		**executePath;

	if (this->_cgiStatus != 200)
		return (content);
	executePath = getExecutorPath(getExecutorLanguage(server));
	if (!executePath)
	{
		std::cerr << "exeuctePath == NULL" << std::endl;
		this->_cgiStatus = 500; // TODO check the status code with path not exist;
		return (content);
	}
	if (request.getRequestBody().size())
		fd = pathExecutor(executePath, request.getRequestBody().at(0).getContent());
	else
		fd = pathExecutor(executePath, "");
	if (fd == -1)
	{
		// TODO check errno with status code;
		this->_cgiStatus = 500;
		return (content);
	}
	content = readFile(fd);
	std::cout << "-> " << content << std::endl;
	close(fd);
	return (content);
}

// TODO change ROOT to real variable;
std::string CommonGatewayInterface::getExecutorLanguage(Config const &server)
{

	if (this->_fileName.find(".py", this->_fileName.length() - 3) != std::string::npos)
		return ("python3");
	else if (this->_fileName.find(".php", this->_fileName.length() - 4) != std::string::npos)
		return ("php");
	else if (this->_fileName.find(".sh", this->_fileName.length() - 3) != std::string::npos)
		return ("bash");
	return(std::string(ROOT) + this->_scriptName);
	// return(server.getRoot() + scriptURI.scriptName);
}

char	**CommonGatewayInterface::getExecutorPath(std::string const &exceLanguage)
{
	char								**execPath;
	int									index;
	std::string							path;
	std::vector<std::string>			splitedPath;
	std::vector<std::string>::iterator	it;

	execPath = NULL;
	index = 0;
	std::cout << "(getExecutorPath) -> |" << exceLanguage << "|" << std::endl;
	if (!access(exceLanguage.c_str(), F_OK | R_OK | X_OK))
	{
		execPath = stringAdd(NULL, const_cast<char*>(exceLanguage.c_str()), SA_NONE);
		return (execPath);
	}
	while (environ[index])
	{
		if (!strncmp(environ[index], "PATH=", 1))
			break ;
		index++;
	}
	if (index == stringsCount(environ))
		return (NULL);
	splitedPath = split(std::string(&environ[index][5]), ':');
	it = splitedPath.begin();
	while (it != splitedPath.end())
	{
		path = *it + "/" + exceLanguage;
		if(!access(path.c_str(), F_OK | R_OK | X_OK))
			break ;
		++it;
	}
	if (it == splitedPath.end())
		return (NULL);
	execPath = stringAdd(execPath, const_cast<char*>(path.c_str()), SA_NONE);
	execPath = stringAdd(execPath, const_cast<char*>(this->_pathFileName.c_str()), SA_NONE);
	return (execPath);
}

#define INFILE_NAME (char *)"t1s3AStaTEmpuLAl1flYinp00tfoRC0mmoNGAt3way1Nte2fac335ucuT3"

int	CommonGatewayInterface::pathExecutor(char **execPath, std::string requestBody)
{
	int				fd[2];
	int				inputFd;
	pid_t			pid;
	int				returnValue = 0;
	std::ofstream	inputFile;
	char			**environment;

	if (!requestBody.empty())
	{
		std::cout << "(pathExecutor) read request body" << std::endl;
		inputFile.open(INFILE_NAME, std::ofstream::trunc | std::ofstream::out);
		if (!inputFile.is_open())
			return (-1);
		inputFile << requestBody;
		inputFile.close();
	}
	if (pipe(fd) < 0)
	{
		std::cerr << "ERROR: usuccess piped." << std::endl;
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{ 
		std::cerr << "ERROR: unsuccess forked." << std::endl;
		return (-1);
	}
	else if (!pid)
	{
		if (!requestBody.empty())
		{
			inputFd = open(INFILE_NAME, O_RDONLY);
			if (inputFd < 0 || dup2(inputFd, STDIN_FILENO) == -1 || dup2(fd[1], STDOUT_FILENO) == -1
				|| close(fd[0]) == -1 || close(fd[1]) == -1 || close(inputFd) == -1)
			exit(1);
		}
		else if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1] == -1))
			exit(1);
		environment = vectorStringToChar(this->_environment);
		std::cerr << "executed" << std::endl;
		execve(execPath[0], execPath, environment);
		std::cerr << "ERORR: unsuccessful execve" << std::endl;
		exit(1);
	}
	else
	{
		waitpid(pid, &returnValue, 0);
		if (!access(INFILE_NAME, F_OK))
			remove(INFILE_NAME);
		close(fd[1]);
		if (returnValue)
			return (-1);
	}
	return (fd[0]);
}


// CONTEXT_DOCUMENT_ROOT = /home/cgi101/public_html
// CONTEXT_PREFIX = 
//? AUTH_TYPE: identification type, if applicable.
//? CONTENT_TYPE: Internet media type of input data if PUT or POST method are used, as provided via HTTP header.
//? CONTENT_LENGTH: similarly, size of input data (decimal, in octets) if provided via HTTP header.
// TODO check argument server is used or not;
void	CommonGatewayInterface::buildEnvironment(Config const &server, HttpRequest const &request)
{
	std::map<std::string, std::string>	requestHeader(request.getRequestHeader());
	int									index;

	index = 0;
	while (environ[index])
		this->_environment.push_back(std::string(environ[index++]));
	
	this->_environment.push_back("HTTP_ACCEPT=" + requestHeader["Accept"]);
	//  HTTP_ACCEPT_CHARSET="ISO-8859-1,utf-8;q=0.7,*;q=0.7"
	this->_environment.push_back("HTTP_ACCEPT_ENCODING=" + requestHeader["Accept-Encoding"]);
	this->_environment.push_back("HTTP_ACCEPT_LANGUAGE=" + requestHeader["Accept-Language"]);
	this->_environment.push_back("HTTP_CONNECTION=" + requestHeader["Connection"]);;
	this->_environment.push_back("HTTP_HOST=" + this->_serverName);
	this->_environment.push_back("HTTP_REFERER=" + requestHeader["Referer"]);
	this->_environment.push_back("HTTP_UPGRADE_INSECURE_REQUESTS=" + requestHeader["Upgrade-Insecure-Requests"]);
	this->_environment.push_back("HTTP_USER_AGENT=" + requestHeader["User-Agent"]);
	this->_environment.push_back("PATH_INFO=" + this->_pathInfo);
	this->_environment.push_back("PATH_TRANSLATED=" + this->_pathTranslated);
	this->_environment.push_back("QUERY_STRING=" + this->_query);
	//*  REMOTE_ADDR="127.0.0.1"
	//*  REMOTE_PORT="63555"
	this->_environment.push_back("REQUEST_METHOD=" + this->_method);
	this->_environment.push_back("REQUEST_SCHEME=" + this->_scheme);
	this->_environment.push_back("REQUEST_URI=" + requestHeader["URL"]);
	this->_environment.push_back("SCRIPT_FILENAM=" + this->_pathFileName);
	this->_environment.push_back("SCRIPT_NAME=" + this->_scriptName);
	//*  SERVER_ADDR="127.0.0.1"
	//  SERVER_ADMIN="(server admin's email address)"
	//*  SERVER_NAME="127.0.0.1"
	this->_environment.push_back("SERVER_PORT=" + this->_serverPort);
	this->_environment.push_back("SERVER_PROTOCOL=" + this->_protocol);
}

// void	CommonGatewayInterface::buildEnvironment(Config const &server, HttpRequest const &request)
// {
// 	int	index;
//
// 	index = 0;
// 	while (environ[index])
// 		this->_environment.push_back(std::string(environ[index++]));
//
// 	// Accept //  HTTP_ACCEPT="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
// 	// this->_environment.push_back("HTTP_ACCEPT=" + request.getRequestHeader().at("Accept"));
// 	this->_environment.push_back("HTTP_ACCEPT=" + request.getRequestHeader()["Accept"]);
//
// 	//  HTTP_ACCEPT_CHARSET="ISO-8859-1,utf-8;q=0.7,*;q=0.7"
// 	// Accept-Encoding //  HTTP_ACCEPT_ENCODING="gzip, deflate, br"
// 	this->_environment.push_back("HTTP_ACCEPT_ENCODING=" + request.getRequestHeader().at("Accept-Encoding"));
// 	// Accept-Language //  HTTP_ACCEPT_LANGUAGE="en-us,en;q=0.5"
// 	this->_environment.push_back("HTTP_ACCEPT_LANGUAGE=" + request.getRequestHeader().at("Accept-Language"));
// 	// Connection //  HTTP_CONNECTION="keep-alive"
// 	this->_environment.push_back("HTTP_CONNECTION=" + request.getRequestHeader().at("Connection"));;
// 	//  HTTP_HOST="example.com"
// 	this->_environment.push_back("HTTP_HOST=" + this->_serverName);
// 	//  HTTP_REFERER = http://www.cgi101.com/book/ch3/text.html
// 	if (request.getRequestHeader().find("Referer") != request.getRequestHeader().end())
// 		this->_environment.push_back("HTTP_REFERER=" + request.getRequestHeader().at("Referer"));
// 	else
// 		this->_environment.push_back("HTTP_REFERER=");
//
// 	// Upgrade-Insecure-Requests //  HTTP_UPGRADE_INSECURE_REQUESTS = 1
// 	this->_environment.push_back("HTTP_UPGRADE_INSECURE_REQUESTS=" + request.getRequestHeader().at("Upgrade-Insecure-Requests"));
// 	// User-Agent //  HTTP_USER_AGENT="Mozilla/5.0 (Windows NT 6.1; WOW64; rv:67.0) Gecko/20100101 Firefox/67.0"
// 	this->_environment.push_back("HTTP_USER_AGENT=" + request.getRequestHeader().at("User-Agent"));
// 	//  PATH_INFO="/foo/bar"
// 	this->_environment.push_back("PATH_INFO=" + this->_pathInfo);
// 	//  PATH_TRANSLATED="C:\Program Files (x86)\Apache Software Foundation\Apache2.4\htdocs\foo\bar"
// 	this->_environment.push_back("PATH_TRANSLATED=" + this->_pathTranslated);
// 	//  QUERY_STRING="var1=value1&var2=with%20percent%20encoding"
// 	this->_environment.push_back("QUERY_STRING=" + this->_query);
// 	//*  REMOTE_ADDR="127.0.0.1"
//
// 	//*  REMOTE_PORT="63555"
//
// 	//  REQUEST_METHOD="GET"
// 	this->_environment.push_back("REQUEST_METHOD=" + this->_method);
// 	// REQUEST_SCHEME = http
// 	this->_environment.push_back("REQUEST_SCHEME=" + this->_scheme);
// 	// Url //  REQUEST_URI="/cgi-bin/printenv.pl/foo/bar?var1=value1&var2=with%20percent%20encoding"
// 	this->_environment.push_back("REQUEST_URI=" + request.getRequestHeader().at("URL"));
// 	//  SCRIPT_FILENAME="C:/Program Files (x86)/Apache Software Foundation/Apache2.4/cgi-bin/printenv.pl"
// 	this->_environment.push_back("SCRIPT_FILENAM=" + this->_pathFileName);
// 	//  SCRIPT_NAME="/cgi-bin/printenv.pl"
// 	this->_environment.push_back("SCRIPT_NAME=" + this->_scriptName);
// 	//*  SERVER_ADDR="127.0.0.1"
//
// 	//  SERVER_ADMIN="(server admin's email address)"
// 	//*  SERVER_NAME="127.0.0.1"
//
// 	//  SERVER_PORT="80"
// 	this->_environment.push_back("SERVER_PORT=" + this->_serverPort);
// 	//  SERVER_PROTOCOL="HTTP/1.1"
// 	this->_environment.push_back("SERVER_PROTOCOL=" + this->_protocol);
// }

// char	**setEnvironmentVariable(Config const &server, t_CGI const &scriptURI, HttpRequest const &request)
// {
// 	char		**cgiEnviron;
// 	int			index;
//
// 	index = 0;
// 	cgiEnviron = new char * [stringsCount(environ) + 18];
// 	while (environ[index])
// 	{
// 		cgiEnviron[index] = environ[index];
// 		index++;
// 	}
// 	// Accept //  HTTP_ACCEPT="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
// 	cgiEnviron[index++] = stringTosChar("HTTP_ACCEPT=" + request.getAcceptStr());
// 	//  HTTP_ACCEPT_CHARSET="ISO-8859-1,utf-8;q=0.7,*;q=0.7"
// 	// Accept-Encoding //  HTTP_ACCEPT_ENCODING="gzip, deflate, br"
// 	cgiEnviron[index++] = stringTosChar("HTTP_ACCEPT_ENCODING=" + request.getEncodeStr());
// 	// Accept-Language //  HTTP_ACCEPT_LANGUAGE="en-us,en;q=0.5"
// 	cgiEnviron[index++] = stringTosChar("HTTP_ACCEPT_LANGUAGE=" + request.getLanguageStr());
// 	// Connection //  HTTP_CONNECTION="keep-alive"
// 	cgiEnviron[index++] = stringTosChar("HTTP_CONNECTION=" + request.getConnection());
// 	//  HTTP_HOST="example.com"
// 	cgiEnviron[index++] = stringTosChar("HTTP_HOST=" + scriptURI.serverName);
// 	//  HTTP_REFERER = http://www.cgi101.com/book/ch3/text.html
// 	cgiEnviron[index++] = stringTosChar("HTTP_REFERER=" + request.getReferer());
// 	// Upgrade-Insecure-Requests //  HTTP_UPGRADE_INSECURE_REQUESTS = 1
// 	cgiEnviron[index++] = stringTosChar("HTTP_UPGRADE_INSECURE_REQUESTS=" + request.getInsecure());
// 	// User-Agent //  HTTP_USER_AGENT="Mozilla/5.0 (Windows NT 6.1; WOW64; rv:67.0) Gecko/20100101 Firefox/67.0"
// 	cgiEnviron[index++] = stringTosChar("HTTP_USER_AGENT=" + request.getAgent());
// 	//  PATH_INFO="/foo/bar"
// 	cgiEnviron[index++] = stringTosChar("PATH_INFO=" + scriptURI.pathInfo);
// 	//  PATH_TRANSLATED="C:\Program Files (x86)\Apache Software Foundation\Apache2.4\htdocs\foo\bar"
// 	cgiEnviron[index++] = stringTosChar("PATH_TRANSLATED=" + scriptURI.pathTranslated);
// 	//  QUERY_STRING="var1=value1&var2=with%20percent%20encoding"
// 	cgiEnviron[index++] = stringTosChar("QUERY_STRING=" + scriptURI.query);
// 	//*  REMOTE_ADDR="127.0.0.1"
//
// 	//*  REMOTE_PORT="63555"
//
// 	//  REQUEST_METHOD="GET"
// 	cgiEnviron[index++] = stringTosChar("REQUEST_METHOD=" + scriptURI.method);
// 	// REQUEST_SCHEME = http
// 	cgiEnviron[index++] = stringTosChar("REQUEST_SCHEME=" + scriptURI.scheme);
// 	// Url //  REQUEST_URI="/cgi-bin/printenv.pl/foo/bar?var1=value1&var2=with%20percent%20encoding"
// 	cgiEnviron[index++] = stringTosChar("REQUEST_URI=" + request.getUrl());
// 	//  SCRIPT_FILENAME="C:/Program Files (x86)/Apache Software Foundation/Apache2.4/cgi-bin/printenv.pl"
// 	cgiEnviron[index++] = stringTosChar("SCRIPT_FILENAM=" + scriptURI.pathFileName);
// 	//  SCRIPT_NAME="/cgi-bin/printenv.pl"
// 	cgiEnviron[index++] = stringTosChar("SCRIPT_NAME=" + scriptURI.scriptName);
// 	//*  SERVER_ADDR="127.0.0.1"
//
// 	//  SERVER_ADMIN="(server admin's email address)"
// 	//*  SERVER_NAME="127.0.0.1"
//
// 	//  SERVER_PORT="80"
// 	cgiEnviron[index++] = stringTosChar("SERVER_PORT=" + scriptURI.serverPort);
// 	//  SERVER_PROTOCOL="HTTP/1.1"
// 	cgiEnviron[index++] = stringTosChar("SERVER_PROTOCOL=" + scriptURI.protocol);
// 	cgiEnviron[index] = NULL;
// 	return (cgiEnviron);
// }

static std::string	readFile(int fd)
{
	int			readByte;
	int			contentSize;
	char		*buffer;
	std::string	content;

	contentSize = 0;
	buffer = new char[READSIZE + 1];
	while (true)
	{
		readByte = read(fd, buffer, READSIZE);
		buffer[readByte] = '\0';
		if (readByte <= 0)
			break ;
		for (int index = 0; index < readByte; ++index)
			content.append(1, buffer[index]);
		contentSize += readByte;
	}
	delete [] buffer;
	return (content);
}

static int	getFileIndex(std::string const &url, std::vector<std::string> &pathSplited);

// TODO server argument didn't use check it later;
void	CommonGatewayInterface::initScriptURI(Config const &server, HttpRequest const &request)
{
	std::vector<std::string>	URISplited;
	std::vector<std::string>	pathSplited;
	std::vector<std::string>	splited;
	int							fileIndex;

	URISplited = split(request.getRequestHeader().at("URL"), '?');
	if (URISplited.size() == 2)
		this->_query = URISplited[1];
	fileIndex = getFileIndex(URISplited[0], pathSplited);
	if (fileIndex == -1)
	{
		this->_cgiStatus = 404;
		return ;
	}
	this->_protocol = "HTTP/1.1";
	this->_scheme = "http";
	this->_method = request.getRequestHeader().at("Method");
	if (pathSplited.size() > 1)
	{
		splited = split(request.getRequestHeader().at("Host"), ':');
		this->_serverName = splited[0];
		if (splited.size() > 1)
			this->_serverPort = splited[1]; //TODO << serverPort should check with port in server
		else
			this->_serverPort = "80";
	}
	this->_fileName = pathSplited[fileIndex];
	for (int i = 0; i <= fileIndex; ++i)
		this->_scriptName += "/" + pathSplited[i];
	for (int i = fileIndex + 1; i < pathSplited.size(); ++i)
		this->_pathInfo += "/" + pathSplited[i];
	this->_pathFileName = ROOT + this->_scriptName;
	this->_pathTranslated = this->_pathFileName + this->_pathInfo;
}

static int	getFileIndex(std::string const &url, std::vector<std::string> &pathSplited)
{
	int				fileIndex;
	DIR				*directory;
	std::string		path;

	fileIndex = -1;
	path.clear();
	pathSplited = split(url, '/');
	path += ROOT;
	for (int i = 0; i < pathSplited.size(); ++i)
	{
		path += "/" + pathSplited[i];
		directory = opendir(path.c_str());
		if (!directory)
		{
			fileIndex = i;
			break ;
		}
		closedir(directory);
	}
	return (fileIndex);
}

std::ostream	&operator<<(std::ostream &out, CommonGatewayInterface const &rhs)
{
	rhs.printValue();
	return (out);
}
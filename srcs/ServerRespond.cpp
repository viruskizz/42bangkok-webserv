/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverRespond.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:36:46 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/12 15:51:28 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerRespond.hpp"

static std::string	initContentType(std::string const &url);
static int			initBodyContent(RequestHeader const &request, Config const &server, char **body);
static std::string	initStatusCode(int code);
static int			initHeaderLength(ServerRespond const &respond);
static int			execCommondGetwayInterface(RequestHeader const &request, Config const &server, char **content);
static int			methodGET(RequestHeader const &request, Config const &server, char **body, int *length);
static int			initHeader(ServerRespond &respond, RequestHeader const &request);

ServerRespond::ServerRespond(void) :
_request(), _contentType(""), _statusCode(""), _httpVersion("HTTP/1.1 "), _bodyContent(NULL), _contentLength(0), _headerLength(0), _cgi(0), _code(0), _respondHeader("")
{
	std::cout << "(ServerRespond) Defualt constructor is called." << std::endl;
}

ServerRespond::ServerRespond(int fd, Config const &server) :
_request(fd), _contentType(""), _statusCode(""), _httpVersion("HTTP/1.1 "), _bodyContent(NULL), _contentLength(0), _headerLength(0), _cgi(0), _code(200), _respondHeader("")
{
	std::cout << "(ServerRespond) Constructor is called." << std::endl;
	if (this->_request.getUrl().find("/cgi-bin/") != std::string::npos)
	{
		this->_contentLength = execCommondGetwayInterface(this->_request, server, &this->_bodyContent);
		if (this->_contentLength >= 0)
			this->_cgi = true;
		else
			std::cout << "set error page with errno" << std::endl;
	}
	else
	{
		this->_contentType = initContentType(this->_request.getUrl());
		if (_request.getMetthod() == "GET")
			this->_code = methodGET(this->_request, server, &this->_bodyContent, &this->_contentLength);
		// else if (_request.getMetthod() == "POST")
		// 	this->_code = methodPOST(this->_request, server, &this->_bodyContent, &this->_contentLength);
		// else if (_request.getMetthod() == "DELETE")
		// 	this->_code = methodDELETE();
		else
			this->_code = 405;
	}
	if (!this->_cgi)
	{
		this->_statusCode = initStatusCode(this->_code);
		this->_headerLength = initHeader(*this, this->_request);
	}
}

ServerRespond::ServerRespond(ServerRespond const &src)
{
	std::cout << "(ServerRespond) Copy constructor is called." << std::endl;
	*this = src;
}

ServerRespond::~ServerRespond(void)
{
	std::cout << "(ServerRepond) Destructor is called." << std::endl;
	if (this->_bodyContent)
		delete [] this->_bodyContent;
}

ServerRespond	&ServerRespond::operator=(ServerRespond const &rhs)
{
	std::cout << "(ServerRepond) Copy Assigntment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_request = rhs._request;
		this->_contentType =  rhs._contentType;
		this->_statusCode = rhs._statusCode;
		this->_httpVersion = rhs._httpVersion;
		this->_contentLength =  rhs._contentLength;
		this->_headerLength = rhs._headerLength;
		this->_code = rhs._code;
		this->_cgi = rhs._cgi;
		if (this->_bodyContent)
			delete [] this->_bodyContent;
		if (rhs._bodyContent)
			this->_bodyContent = stringDuplicate(rhs._bodyContent);
		else
			this->_bodyContent = NULL;
	}
	return (*this);
}

RequestHeader const	&ServerRespond::getRequest(void) const
{
	return (this->_request);
}

std::string const	&ServerRespond::getContentType(void) const
{
	return (this->_contentType);
}

std::string const	&ServerRespond::getStatusCode(void) const
{
	return (this->_statusCode);
}

std::string const	&ServerRespond::getHttpVersion(void) const
{
	return (this->_httpVersion);
}

char const	*ServerRespond::getBodyContent(void) const
{
	return (this->_bodyContent);
}

int	ServerRespond::getBodyLength(void) const
{
	return (this->_contentLength);
}

int	ServerRespond::getHeaderLength(void) const
{
	return (this->_headerLength);
}

int	ServerRespond::getRespondLength(void) const
{
	return (this->_headerLength + this->_contentLength);
}

void	ServerRespond::sendRepond(int socket) const
{
	if (this->_cgi)
		write(socket, this->_bodyContent, this->_contentLength);
	else
		write(socket, this->getRespond().c_str(), this->_headerLength + this->_contentLength);
	
}

std::string	ServerRespond::getRespond(void) const
{
	std::string	serverRespond;

	serverRespond = this->_respondHeader;
	std::cout << this->_respondHeader <<std::endl;
	for (int i = 0; i < this->_contentLength; ++i)
		serverRespond.append(1, this->_bodyContent[i]);
	return (serverRespond);
}

void	ServerRespond::setRespondHeader(std::string const &respondHeader)
{
	this->_respondHeader = respondHeader;
}

int		ServerRespond::getCode(void) const
{
	return (this->_code);
}

static int	initHeader(ServerRespond &respond, RequestHeader const &request)
{
	std::string	respondHeader;

	respondHeader.clear();
	respondHeader += respond.getHttpVersion() + respond.getStatusCode() + BREAK_LINE; 
	respondHeader += "Content-Type: " + respond.getContentType() + BREAK_LINE; 
	if (respond.getBodyLength() >= 0)
		respondHeader += "Content-Length: " + intToString(respond.getBodyLength()) + BREAK_LINE;
	if (respond.getCode() == 405)
		respondHeader += std::string("Allow: GET, POST, DELETE") + BREAK_LINE;
	if (request.getRangeStr().size())
		respondHeader += "Content-Range: bytes " + request.getRangeStr() + "/*" + BREAK_LINE;
	respondHeader += BREAK_LINE;
	respond.setRespondHeader(respondHeader);
	return (respondHeader.size());
}

std::ostream	&operator<<(std::ostream &out, ServerRespond const &rhs)
{
	out << rhs.getHttpVersion() << rhs.getStatusCode() << std::endl;
	out << "Content-Type: " << rhs.getContentType() << std::endl;
	out << "Content-Length: " << rhs.getBodyLength() << std::endl;
	out << "Header-Length: " << rhs.getHeaderLength() << std::endl;
	out << "Length: " << rhs.getRespondLength() << std::endl;
	out << std::endl;
	for (int i = 0; i < rhs.getBodyLength(); ++i)
		out << rhs.getBodyContent()[i];
	out << std::endl;
	return (out);
}

static int	getFileDiscriptor(RequestHeader const &request, Config const &server);

static int methodGET(RequestHeader const &request, Config const &server, char **body, int *length)
{
	char						*content;
	std::vector<std::string>	splited;
	int							start;
	int							end;
	int							size;

	*length = initBodyContent(request, server, body);
	if (*length == -1)
		return (404);
	if (request.getRangeStr().size())
	{
		content = *body;
		// for (std::vector<std::string>::iterator it = request.getRange().begin();
			// it != request.getRange().end(); ++it)
		{
			splited = split(request.getRange()[0], '-');
			start = stringToint(splited[0]);
			end = stringToint(splited[1]);
			if (start < 0 || start > *length || start > end || end < 0 || end > *length)
				return (200);
			size = end - start;
			*body = new char[size + 1];
			for (int i = start; i < end; ++i)
				*body[i] = content[i];
			delete [] content;
			*length = size;
		}
		return(206);
	}
 	return (200);
}

// static int	methodPOST(RequestHeader const &request, Config const &server, char **body, int *length)
// {
// 	if (request.getContentType() == "application/x-www-form-urlencoded")
// 	{
// 		return (200);
// 	}
// 	else if (request.getContentType() == "multipart/form-data")
// 	{

// 	}
// 	else if (request.getContentType() == "text/plain")
// 	{

// 	}
// 	return (405);
// }

static int initBodyContent(RequestHeader const &request, Config const &server, char **body)
{
	char	*buffer;
	int		readByte;
	int		bodySize;
	int		fd;

	bodySize = 0;
	fd = getFileDiscriptor(request, server);
	if (fd < 0)
	{
		perror("webserv :ERROR"); // ? maybe need return errro number for respond header.
		std::cout << "errno: " << errno << std::endl;
		return (-1);
	}
	buffer = new char[READSIZE + 1];
	while (true)
	{
		readByte = read(fd, buffer, READSIZE);
		buffer[readByte] = '\0';
		if (readByte <= 0)
			break ;
		*body = strjoin(*body,bodySize, buffer, readByte, SJ_FFIR);
		bodySize += readByte;
	}
	delete [] buffer;
	close(fd);
	return (bodySize);
}

static int	getFileDiscriptor(RequestHeader const &request, Config const &server)
{
	int		fd;
	char	*fileName;
	int		count;

	fileName = NULL;
	count = 0;
	if (request.getUrl() == "/")
	{
		while (count < server.getIndex().size())
		{
			fileName = NULL;
			fileName = strjoin(ROOT, const_cast<char *>(server.getIndex()[count].c_str()), SJ_NONE);
			std::cout << "openFirst-> " << fileName << std::endl;
			fd = open(fileName, O_RDONLY);
			delete [] fileName;
			if (fd >= 3)
				break ;
			count++;
		}
	}
	else
	{
		fileName = strjoin(ROOT, const_cast<char *>(request.getUrl().c_str()), SJ_NONE);
		std::cout << "openSeccond-> " << fileName << std::endl;
		fd = open(fileName, O_RDONLY);
		delete [] fileName;
	}
	return (fd);
}

extern char **environ;

static std::string	getExecutorLanguage(t_CGI scriptURI);
static char			**getExecutorPath(std::string const &exceLanguage, t_CGI const &scriptURI);
static t_CGI		initScriptURI(std::string const &url, RequestHeader const &request);
static int			pathExecutor(char **execPath, Config const &server,  t_CGI const &scriptURI, RequestHeader const &request);
static int			readFile(int fd, char **content);
static bool			contentHeaderCheck(char *content);

static int	execCommondGetwayInterface(RequestHeader const &request, Config const &server, char **content)
{
	int							fd;
	char						**executePath;
	t_CGI						scriptURI;
	std::vector<std::string>	requestPath;
	std::vector<std::string>	requestQuery;
	int							contentSize;

	executePath = NULL;
	contentSize = 0;
	scriptURI = initScriptURI(request.getUrl(), request);
	scriptURI.print();
	executePath = getExecutorPath(const_cast<char*>(getExecutorLanguage(scriptURI).c_str()), scriptURI);
	if (!executePath)
	{
		std::cerr << "exeuctePath == NULL" << std::endl;
		return (-1);
	}
	fd = pathExecutor(executePath, server, scriptURI, request);
	if (fd == -1)
		exit (-1);
	contentSize = readFile(fd, content);
	close(fd);
	return (contentSize);
}

static std::string getExecutorLanguage(t_CGI scriptURI)
{
	std::string	fileName;

	fileName = scriptURI.fileName;
	if (fileName.find(".py", fileName.length() - 3) != std::string::npos)
		return ("python3");
	return(std::string(ROOT) + scriptURI.scriptName);
}

static char	**getExecutorPath(std::string const &exceLanguage, t_CGI const &scriptURI)
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
	execPath = stringAdd(execPath, const_cast<char*>(scriptURI.pathFileName.c_str()), SA_NONE);
	return (execPath);
}

static int	getFileIndex(std::string const &url, std::vector<std::string> &pathSplited);

static t_CGI	initScriptURI(std::string const &url, RequestHeader const &request)
{
	t_CGI						scriptURI;
	std::vector<std::string>	URISplited;
	std::vector<std::string>	pathSplited;
	std::vector<std::string>	splited;
	int							fileIndex;

	memset(&scriptURI, '\0', sizeof(t_CGI));
	URISplited = split(url, '?');
	if (URISplited.size() > 1)
		scriptURI.query = URISplited[1];
	fileIndex = getFileIndex(URISplited[0], pathSplited);
	// pathSplited = split(URISplited[0], '/');
	// for (int i = 0; i < pathSplited.size(); ++i)
	// {
	// 	if (i != 1 && pathSplited[i].find('.') != std::string::npos)
	// 	{
	// 		fileIndex = i;
	// 		break ;
	// 	}
	// }
	// if (fileIndex == -1)
	// {
	// 	fileIndex = pathSplited.size() - 1;
	//
	// }
	scriptURI.protocol = "HTTP/1.1";
	scriptURI.scheme = "http";
	scriptURI.method = request.getMetthod();
	if (pathSplited.size() > 1)
	{
		splited = split(request.getHost(), ':');
		scriptURI.serverName = splited[0];
		if (splited.size() > 1)
			scriptURI.serverPort = splited[1];
		else
			scriptURI.serverPort = "80";
	}
	scriptURI.fileName = pathSplited[fileIndex];
	for (int i = 0; i <= fileIndex; ++i)
		scriptURI.scriptName += "/" + pathSplited[i];
	for (int i = fileIndex + 1; i < pathSplited.size(); ++i)
		scriptURI.pathInfo += "/" + pathSplited[i];
	scriptURI.pathFileName = ROOT + scriptURI.scriptName;
	scriptURI.pathTranslated = scriptURI.pathFileName + scriptURI.pathInfo;
	return (scriptURI);
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
	if (fileIndex == -1)
		fileIndex = pathSplited.size() - 1;
	return (fileIndex);
}

static char			**setEnvironmentVariable(Config const &server, t_CGI const &scriptURI, RequestHeader const &request);

static int	pathExecutor(char **execPath, Config const &server, t_CGI const &scriptURI, RequestHeader const &request)
{
	int		fd[2];
	pid_t	pid;
	int		returnValue = 0;

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
		if (dup2(fd[1], STDOUT_FILENO) == -1 || close(fd[0]) == -1 || close(fd[1] == -1))
			exit(1);
		environ = setEnvironmentVariable(server, scriptURI, request);
		std::cerr << "executed" << std::endl;
		execve(execPath[0], execPath, environ);
		std::cerr << "ERORR: unsuccessful execve" << std::endl;
		exit(1);
	}
	else
	{
		waitpid(pid, &returnValue, WNOHANG);
		if (returnValue)
			return (-1);
		close(fd[1]);
	}
	return (fd[0]);
}

// CONTEXT_DOCUMENT_ROOT = /home/cgi101/public_html
// CONTEXT_PREFIX = 
//? AUTH_TYPE: identification type, if applicable.
//? CONTENT_TYPE: Internet media type of input data if PUT or POST method are used, as provided via HTTP header.
//? CONTENT_LENGTH: similarly, size of input data (decimal, in octets) if provided via HTTP header.

char	**setEnvironmentVariable(Config const &server, t_CGI const &scriptURI, RequestHeader const &request)
{
	char		**cgiEnviron;
	int			index;

	index = 0;
	cgiEnviron = new char * [stringsCount(environ) + 18];
	while (environ[index])
	{
		cgiEnviron[index] = environ[index];
		index++;
	}
	// Accept //  HTTP_ACCEPT="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
	cgiEnviron[index++] = stringTosChar("HTTP_ACCEPT=" + request.getAcceptStr());
	//  HTTP_ACCEPT_CHARSET="ISO-8859-1,utf-8;q=0.7,*;q=0.7"
	// Accept-Encoding //  HTTP_ACCEPT_ENCODING="gzip, deflate, br"
	cgiEnviron[index++] = stringTosChar("HTTP_ACCEPT_ENCODING=" + request.getEncodeStr());
	// Accept-Language //  HTTP_ACCEPT_LANGUAGE="en-us,en;q=0.5"
	cgiEnviron[index++] = stringTosChar("HTTP_ACCEPT_LANGUAGE=" + request.getLanguageStr());
	// Connection //  HTTP_CONNECTION="keep-alive"
	cgiEnviron[index++] = stringTosChar("HTTP_CONNECTION=" + request.getConnection());
	//  HTTP_HOST="example.com"
	cgiEnviron[index++] = stringTosChar("HTTP_HOST=" + scriptURI.serverName);
	//  HTTP_REFERER = http://www.cgi101.com/book/ch3/text.html
	cgiEnviron[index++] = stringTosChar("HTTP_REFERER=" + request.getReferer());
	// Upgrade-Insecure-Requests //  HTTP_UPGRADE_INSECURE_REQUESTS = 1
	cgiEnviron[index++] = stringTosChar("HTTP_UPGRADE_INSECURE_REQUESTS=" + request.getInsecure());
	// User-Agent //  HTTP_USER_AGENT="Mozilla/5.0 (Windows NT 6.1; WOW64; rv:67.0) Gecko/20100101 Firefox/67.0"
	cgiEnviron[index++] = stringTosChar("HTTP_USER_AGENT=" + request.getAgent());
	//  PATH_INFO="/foo/bar"
	cgiEnviron[index++] = stringTosChar("PATH_INFO=" + scriptURI.pathInfo);
	//  PATH_TRANSLATED="C:\Program Files (x86)\Apache Software Foundation\Apache2.4\htdocs\foo\bar"
	cgiEnviron[index++] = stringTosChar("PATH_TRANSLATED=" + scriptURI.pathTranslated);
	//  QUERY_STRING="var1=value1&var2=with%20percent%20encoding"
	cgiEnviron[index++] = stringTosChar("QUERY_STRING=" + scriptURI.query);
	//*  REMOTE_ADDR="127.0.0.1"

	//*  REMOTE_PORT="63555"

	//  REQUEST_METHOD="GET"
	cgiEnviron[index++] = stringTosChar("REQUEST_METHOD=" + scriptURI.method);
	// REQUEST_SCHEME = http
	cgiEnviron[index++] = stringTosChar("REQUEST_SCHEME=" + scriptURI.scheme);
	// Url //  REQUEST_URI="/cgi-bin/printenv.pl/foo/bar?var1=value1&var2=with%20percent%20encoding"
	cgiEnviron[index++] = stringTosChar("REQUEST_URI=" + request.getUrl());
	//  SCRIPT_FILENAME="C:/Program Files (x86)/Apache Software Foundation/Apache2.4/cgi-bin/printenv.pl"
	cgiEnviron[index++] = stringTosChar("SCRIPT_FILENAM=" + scriptURI.pathFileName);
	//  SCRIPT_NAME="/cgi-bin/printenv.pl"
	cgiEnviron[index++] = stringTosChar("SCRIPT_NAME=" + scriptURI.scriptName);
	//*  SERVER_ADDR="127.0.0.1"

	//  SERVER_ADMIN="(server admin's email address)"
	//*  SERVER_NAME="127.0.0.1"

	//  SERVER_PORT="80"
	cgiEnviron[index++] = stringTosChar("SERVER_PORT=" + scriptURI.serverPort);
	//  SERVER_PROTOCOL="HTTP/1.1"
	cgiEnviron[index++] = stringTosChar("SERVER_PROTOCOL=" + scriptURI.protocol);
	cgiEnviron[index] = NULL;
	return (cgiEnviron);
}


// static int	setEnvironmentVariable(Config const &server, t_CGI const &scriptURI, RequestHeader const &request)
// {
// 	char	**environment;
//
// 	environment = stringAdd(environ, NULL, SA_NONE);
// 	// Accept //  HTTP_ACCEPT="text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_ACCEPT=" + request.getAcceptStr()).c_str()), SA_FSTS);
// 	//  HTTP_ACCEPT_CHARSET="ISO-8859-1,utf-8;q=0.7,*;q=0.7"
// 	// Accept-Encoding //  HTTP_ACCEPT_ENCODING="gzip, deflate, br"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_ACCEPT_ENCODING=" + request.getEncodeStr()).c_str()), SA_FSTS);
// 	// Accept-Language //  HTTP_ACCEPT_LANGUAGE="en-us,en;q=0.5"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_ACCEPT_LANGUAGE=" + request.getLanguageStr()).c_str()), SA_FSTS);
// 	// Connection //  HTTP_CONNECTION="keep-alive"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_CONNECTION=" + request.getConnection()).c_str()), SA_FSTS);
// 	//  HTTP_HOST="example.com"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_HOST=" + scriptURI.serverName).c_str()), SA_FSTS);
// 	//  HTTP_REFERER = http://www.cgi101.com/book/ch3/text.html
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_REFERER=" + request.getReferer()).c_str()), SA_FSTS);
// 	// Upgrade-Insecure-Requests //  HTTP_UPGRADE_INSECURE_REQUESTS = 1
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_UPGRADE_INSECURE_REQUESTS=" + request.getInsecure()).c_str()), SA_FSTS);
// 	// User-Agent //  HTTP_USER_AGENT="Mozilla/5.0 (Windows NT 6.1; WOW64; rv:67.0) Gecko/20100101 Firefox/67.0"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("HTTP_USER_AGENT=" + request.getAgent()).c_str()), SA_FSTS);
// 	//  PATH_INFO="/foo/bar"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("PATH_INFO=" + scriptURI.pathInfo).c_str()), SA_FSTS);
// 	//  PATH_TRANSLATED="C:\Program Files (x86)\Apache Software Foundation\Apache2.4\htdocs\foo\bar"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("PATH_TRANSLATED=" + scriptURI.pathTranslated).c_str()), SA_FSTS);
// 	//  QUERY_STRING="var1=value1&var2=with%20percent%20encoding"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("QUERY_STRING=" + scriptURI.query).c_str()), SA_FSTS);
// 	//*  REMOTE_ADDR="127.0.0.1"
//
// 	//*  REMOTE_PORT="63555"
//
// 	//  REQUEST_METHOD="GET"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("REQUEST_METHOD=" + scriptURI.method).c_str()), SA_FSTS);
// 	// REQUEST_SCHEME = http
// 	environment = stringAdd(environment, const_cast<char *>(std::string("REQUEST_SCHEME=" + scriptURI.scheme).c_str()), SA_FSTS);
// 	// Url //  REQUEST_URI="/cgi-bin/printenv.pl/foo/bar?var1=value1&var2=with%20percent%20encoding"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("REQUEST_URI=" + request.getUrl()).c_str()), SA_FSTS);
// 	//  SCRIPT_FILENAME="C:/Program Files (x86)/Apache Software Foundation/Apache2.4/cgi-bin/printenv.pl"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("SCRIPT_FILENAM=" + scriptURI.pathFileName).c_str()), SA_FSTS);
// 	//  SCRIPT_NAME="/cgi-bin/printenv.pl"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("SCRIPT_NAME=" + scriptURI.scriptName).c_str()), SA_FSTS);
// 	//*  SERVER_ADDR="127.0.0.1"
//
// 	//  SERVER_ADMIN="(server admin's email address)"
// 	//*  SERVER_NAME="127.0.0.1"
//
// 	//  SERVER_PORT="80"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("SERVER_PORT=" + scriptURI.serverPort).c_str()), SA_FSTS);
// 	//  SERVER_PROTOCOL="HTTP/1.1"
// 	environment = stringAdd(environment, const_cast<char *>(std::string("SERVER_PROTOCOL=" + scriptURI.protocol).c_str()), SA_FSTS);
// 	environ = environment;
// 	return (1);
// }

static int	readFile(int fd, char **content)
{
	int		readByte;
	int		contentSize;
	char	*buffer;

	contentSize = 0;
	buffer = new char[READSIZE + 1];
	while (true)
	{
		readByte = read(fd, buffer, READSIZE);
		buffer[readByte] = '\0';
		if (readByte <= 0)
			break ;
		*content = strjoin(*content, contentSize, buffer, readByte, SJ_FFIR);
		contentSize += readByte;
	}
	delete [] buffer;
	return (contentSize);
}

static std::string	initStatusCode(int code)
{
	if (code >= 100 && code <= 103)
	{
		if (code == 100)
			return("100 Continue");
		else if (code == 101)
			return("101 Switching Protocols");
		else if (code == 102)
			return("102 Processing");
		else if (code == 103)
			return("103 Early Hints");
	}
	else if (code >= 200 && code <= 208 || code == 226)
	{
		if (code == 200)
			return("200 OK");
		else if (code == 201)
			return("201 Created");
		else if (code == 202)
			return("202 Accepted");
		else if (code == 203)
			return("203 Non-Authoritative Information");
		else if (code == 204)
			return("204 No Content");
		else if (code == 205)
			return("205 Reset Content");
		else if (code == 206)
			return("206 Partial Content");
		else if (code == 207)
			return("207 Multi-Status");
		else if (code == 208)
			return("208 Already Reported");
		else if (code == 226)
			return("226 IM Used");
	}
	else if (code >= 300 && code <= 308)
	{
		if (code == 300)
			return("300 Multiple Choices");
		else if (code == 301)
			return("301 Moved Permanently");
		else if (code == 302)
			return("302 Found");
		else if (code == 303)
			return("303 See Other");
		else if (code == 304)
			return("304 Not Modified");
		else if (code == 305)
			return("305 Use Proxy");
		else if (code == 306)
			return("306 unused");
		else if (code == 307)
			return("307 Temporary Redirect");
		else if (code == 308)
			return("308 Permanent Redirect");
	}
	else if (code >= 400 && code <= 418 || code >= 421 && code <= 429 || code == 431 || code == 451)
	{
		if (code == 400)
			return("400 Bad Request");
		else if (code == 401)
			return("401 Unauthorized");
		else if (code == 402)
			return("402 Payment Required");
		else if (code == 403)
			return("403 Forbidden");
		else if (code == 404)
			return("404 Not Found");
		else if (code == 405)
			return("405 Method Not Allowed");
		else if (code == 406)
			return("406 Not Acceptable");
		else if (code == 407)
			return("407 Proxy Authentication Required");
		else if (code == 408)
			return("408 Request Timeout");
		else if (code == 409)
			return("409 Conflict");
		else if (code == 410)
			return("410 Gone");
		else if (code == 411)
			return("411 Length Required");
		else if (code == 412)
			return("412 Precondition Failed");
		else if (code == 413)
			return("413 Payload Too Large");
		else if (code == 414)
			return("414 URI Too Long");
		else if (code == 415)
			return("415 Unsupported Media Type");
		else if (code == 416)
			return("416 Range Not Satisfiable");
		else if (code == 417)
			return("417 Expectation Failed");
		else if (code == 418)
			return("418 I'm a teapot");
		else if (code == 421)
			return("421 Misdirected Request");
		else if (code == 422)
			return("422 Unprocessable Content");
		else if (code == 423)
			return("423 Locked");
		else if (code == 424)
			return("424 Failed Dependency");
		else if (code == 425)
			return("425 Too Early");
		else if (code == 426)
			return("426 Upgrade Required");
		else if (code == 428)
			return("428 Precondition Required");
		else if (code == 429)
			return("429 Too Many Requests");
		else if (code == 431)
			return("431 Request Header Fields Too Large");
		else if (code == 451)
			return("451 Unavailable For Legal Reasons");
	}
	else if (code >= 500 && code <= 508 || code == 510 || code == 511)
	{
		if (code == 500)
			return("500 Internal Server Error");
		else if (code == 501)
			return("501 Not Implemented");
		else if (code == 502)
			return("502 Bad Gateway");
		else if (code == 503)
			return("503 Service Unavailable");
		else if (code == 504)
			return("504 Gateway Timeout");
		else if (code == 505)
			return("505 HTTP Version Not Supported");
		else if (code == 506)
			return("506 Variant Also Negotiates");
		else if (code == 507)
			return("507 Insufficient Storage");
		else if (code == 508)
			return("508 Loop Detected");
		else if (code == 510)
			return("510 Not Extended");
		else if (code == 511)
			return("511 Network Authentication Required");
	}
	return ("");
}

static std::string	initContentType(std::string const &url)
{
	std::string	contentType;
	int			checkPosition;

	checkPosition = url.length();
	if (url.find(".aac", checkPosition - 4) != std::string::npos)
		contentType = "audio/aac";
	else if (url.find(".abw", checkPosition - 4) != std::string::npos)
		contentType = "application/x-abiword";
	else if (url.find(".arc", checkPosition - 4) != std::string::npos)
		contentType = "application/x-freearc";
	else if (url.find(".avif", checkPosition - 5) != std::string::npos)
		contentType = "image/avif";
	else if (url.find(".avi", checkPosition - 4) != std::string::npos)
		contentType = "video/x-msvideo";
	else if (url.find(".azw", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.amazon.ebook";
	else if(url.find(".bin", checkPosition - 4) != std::string::npos)
		contentType = "application/octet-stream";
	else if (url.find(".bmp", checkPosition - 4) != std::string::npos)
		contentType = "image/bmp";
	else if (url.find(".bz", checkPosition - 4) != std::string::npos)
		contentType = "application/x-bzip";
	else if (url.find(".bz2", checkPosition - 4) != std::string::npos)
		contentType = "application/x-bzip2";
	else if (url.find(".cda", checkPosition - 4) != std::string::npos)
		contentType = "application/x-cdf";
	else if (url.find(".csh", checkPosition - 4) != std::string::npos)
		contentType = "application/x-csh";
	else if (url.find(".css", checkPosition - 4) != std::string::npos)
		contentType = "text/css";
	else if (url.find(".csv", checkPosition - 4) != std::string::npos)
		contentType = "text/csv";
	else if (url.find(".doc", checkPosition - 4) != std::string::npos)
		contentType = "application/msword";
	else if (url.find(".docx", checkPosition - 5) != std::string::npos)
		contentType = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	else if (url.find(".eot", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.ms-fontobject";
	else if (url.find(".epub", checkPosition - 5) != std::string::npos)
		contentType = "application/epub+zip";
	else if (url.find(".gz", checkPosition - 3) != std::string::npos)
		contentType = "application/gzip";
	else if (url.find(".gif", checkPosition - 4) != std::string::npos)
		contentType = "image/gif";
	else if (url.find(".htm", checkPosition - 4)!= std::string::npos
		|| url.find(".html", checkPosition - 5) != std::string::npos)
		contentType = "text/html";
	else if (url.find(".ico", checkPosition - 4) != std::string::npos)
		contentType = "image/vnd.microsoft.icon";
	else if (url.find(".ics", checkPosition - 4) != std::string::npos)
		contentType = "text/calendar";
	else if (url.find(".jar", checkPosition - 4) != std::string::npos)
		contentType = "application/java-archive";
	else if (url.find(".jpeg", checkPosition - 5) != std::string::npos
		|| url.find(".jpg", checkPosition - 4) != std::string::npos)
		contentType = "image/jpeg";
	else if (url.find(".js", checkPosition - 3) != std::string::npos
		|| url.find(".mjs", checkPosition - 4) != std::string::npos)
		contentType = "text/javascript";
	else if (url.find(".json", checkPosition - 5) != std::string::npos)
		contentType = "application/json";
	else if (url.find(".jsonld", checkPosition - 7) != std::string::npos)
		contentType = "application/ld+json";
	else if (url.find(".mid", checkPosition - 4) != std::string::npos
		|| url.find(".midi", checkPosition - 5) != std::string::npos)
		contentType = "audio/midi";
	else if (url.find(".mp3", checkPosition - 4) != std::string::npos)
		contentType = "audio/mpeg";
	else if (url.find(".mp4", checkPosition - 4) != std::string::npos)
		contentType = "video/mp4";
	else if (url.find(".mpeg", checkPosition - 5) != std::string::npos)
		contentType = "video/mpeg";
	else if (url.find(".mpkg", checkPosition - 5) != std::string::npos)
		contentType = "application/vnd.apple.installer+xml";
	else if (url.find(".odp", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.oasis.opendocument.presentation";
	else if (url.find(".ods", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.oasis.opendocument.spreadsheet";
	else if (url.find(".odt", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.oasis.opendocument.text";
	else if (url.find(".oga", checkPosition - 4) != std::string::npos)
		contentType = "audio/ogg";
	else if (url.find(".ogv", checkPosition - 4) != std::string::npos)
		contentType = "video/ogg";
	else if (url.find(".ogx", checkPosition - 4) != std::string::npos)
		contentType = "application/ogg";
	else if (url.find(".opus", checkPosition - 5) != std::string::npos)
		contentType = "audio/opus";
	else if (url.find(".otf", checkPosition - 4) != std::string::npos)
		contentType = "font/otf";
	else if (url.find(".png", checkPosition - 4) != std::string::npos)
		contentType = "image/png";
	else if (url.find(".pdf", checkPosition - 4) != std::string::npos)
		contentType = "application/pdf";
	else if (url.find(".php", checkPosition - 4) != std::string::npos)
		contentType = "application/x-httpd-php";
	else if (url.find(".ppt", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.ms-powerpoint";
	else if (url.find(".pptx", checkPosition - 5) != std::string::npos)
		contentType = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	else if (url.find(".rar", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.rar";
	else if (url.find(".rtf", checkPosition - 4) != std::string::npos)
		contentType = "application/rtf";
	else if (url.find(".sh", checkPosition - 3) != std::string::npos)
		contentType = "application/x-sh";
	else if (url.find(".svg", checkPosition - 4) != std::string::npos)
		contentType = "image/svg+xml";
	else if (url.find(".tar", checkPosition - 4) != std::string::npos)
		contentType = "application/x-tar";
	else if (url.find(".tif", checkPosition - 4) != std::string::npos
		|| url.find(".tiff", checkPosition - 5) != std::string::npos)
		contentType = "image/tiff";
	else if (url.find(".ts", checkPosition - 3) != std::string::npos)
		contentType = "video/mp2t";
	else if (url.find(".ttf", checkPosition - 4) != std::string::npos)
		contentType = "font/ttf";
	else if (url.find(".txt", checkPosition - 4) != std::string::npos)
		contentType = "text/plain";
	else if (url.find(".vsd", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.visio";
	else if (url.find(".wav", checkPosition - 4) != std::string::npos)
		contentType = "audio/wav";
	else if (url.find(".weba", checkPosition - 5) != std::string::npos)
		contentType = "audio/webm";
	else if (url.find(".webm", checkPosition - 5) != std::string::npos)
		contentType = "video/webm";
	else if (url.find(".webp", checkPosition - 5) != std::string::npos)
		contentType = "image/webp";
	else if (url.find(".woff", checkPosition - 5) != std::string::npos)
		contentType = "font/woff";
	else if (url.find(".woff2", checkPosition - 6) != std::string::npos)
		contentType = "font/woff2";
	else if (url.find(".xhtml", checkPosition - 6) != std::string::npos)
		contentType = "application/xhtml+xml";
	else if (url.find(".xls", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.ms-excel";
	else if (url.find(".xlsx", checkPosition - 5) != std::string::npos)
		contentType = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	else if (url.find(".xml", checkPosition - 4) != std::string::npos)
		contentType = "application/xml";
	else if (url.find(".xul", checkPosition - 4) != std::string::npos)
		contentType = "application/vnd.mozilla.xul+xml";
	else if (url.find(".zip", checkPosition - 4) != std::string::npos)
		contentType = "application/zip";
	else if (url.find(".3gp", checkPosition - 4) != std::string::npos)
		contentType = "video/3gpp";
	else if (url.find(".3g2", checkPosition - 4) != std::string::npos)
		contentType = "video/3gpp2";
	else if (url.find(".7z", checkPosition - 3) != std::string::npos)
		contentType = "application/x-7z-compressed";
	else
		contentType = "text/plain";
	return (contentType);
}

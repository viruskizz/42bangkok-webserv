/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 22:43:02 by sharnvon          #+#    #+#             */
/*   Updated: 2023/10/26 03:18:28 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonGatewayInterface.hpp"
#include <fstream>

CommonGatewayInterface::CommonGatewayInterface(void) :
_protocol(""), _scheme(""), _serverPort(""), _method(""), _pathInfo(""), _pathTranslated(""),
_pathFileName(""), _scriptName(""), _fileName(""), _query(""), _clientName(""), _clientPort(""),
_cgiStatus(200)
{
	// std::cout << "(CommonGatewayInterface) Defualt constructor is called." << std::endl;
}

CommonGatewayInterface::CommonGatewayInterface(Config const &server, HttpRequest const &request) : _cgiStatus(200)
{
	// std::cout << "(CommonGatewayInterface) Constructor is called." << std::endl;
	(void) server;
	this->initScriptURI(request);
	this->buildEnvironment(request);
}

CommonGatewayInterface::CommonGatewayInterface(CommonGatewayInterface const & src)
{
	// std::cout << "(CommonGatewayInterface) Copy constructor is called." << std::endl;
	*this = src;
}

CommonGatewayInterface::~CommonGatewayInterface(void)
{
	// std::cout << "(CommonGatewayInterface) Destructor is called." << std::endl;
}

CommonGatewayInterface	&CommonGatewayInterface::operator=(CommonGatewayInterface const &rhs)
{
	// std::cout << "(CommonGatewayInterface) Copy assiagnment operator is called." << std::endl;
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
static bool	checkCGIHeader(std::string &content, HttpRequest const &request);
static int	findBreakLine(std::vector<std::string> const &splited);

std::string	CommonGatewayInterface::executeCGI(Config const &server, HttpRequest const &request)
{
	std::string	content;
	int			fd;
	char		**executePath;

	executePath = getExecutorPath(getExecutorLanguage(server, request));
	if (!executePath)
	{
		this->_cgiStatus = 404;
		return (content);
	}
	if (!request.getRequestBody().size())
	{
		fd = pathExecutor(executePath, request.getRequestBody(), -1, request.getCGITimeout());
		if (fd == -1)
			return (this->errnoCheck());
		content = readFile(fd);
		close(fd);
	}
	else
		content = this->executeWithBody(request, executePath);
	if (!request.getFileCGI().empty() && !checkCGIHeader(content, request))
		this->_cgiStatus = 500;
	return (content);
}

std::string	 CommonGatewayInterface::executeWithBody(HttpRequest const &request, char **executePath)
{
	std::string			body;
	std::string			content;
	std::vector<string>	splited;
	int					fd;
	size_t				breakLine;

	body.clear();
	for (size_t index = 0; index < request.getRequestBody().size(); ++index)
	{
		fd = pathExecutor(executePath, request.getRequestBody(), index, request.getCGITimeout());
		if (fd == -1)
			return (this->errnoCheck());
		content = readFile(fd);
		splited = split(content, '\n');
		breakLine = findBreakLine(splited);
		while (true)
		{
			body += splited.at(breakLine++);
			if (breakLine >= splited.size())
				break ;
		}
		close(fd);
	}
	if (!request.getFileCGI().empty() && splited.size())
	{
		content.clear();
		breakLine = !findBreakLine(splited) ? 0 : findBreakLine(splited) - 1;
		for (size_t i = 0; i < breakLine; ++i)
			content += stringTrim(splited.at(i), "\r\n") + "\r\n";
		if (!isStringFound(content, "Content-Length: "))
			content += "Content-Length: " +  intToString(body.size()) + "\r\n";
		content += "\r\n";
		content += body + "\r\n";
	}
	return (content);
}

std::string	CommonGatewayInterface::errnoCheck(void)
{
	if (this->_cgiStatus == 200)
	{
		if (errno == 2)
			this->_cgiStatus = 404;
		else if (errno == 13)
			this->_cgiStatus = 401;
		else
			this->_cgiStatus = 500;
	}
	return ("");
}

static int	findBreakLine(std::vector<std::string> const &splited)
{
	size_t	count;

	count = 0;
	while (count < splited.size())
	{
		if (splited.at(count) == "\r")
			return (++count);
		count++;
	}
	return (0);
}

std::string CommonGatewayInterface::getExecutorLanguage(Config const &server, HttpRequest const &request)
{
	(void) server;
	if (request.getPath().find(".py", request.getPath().length() - 3) != std::string::npos)
		return ("python3");
	else if (request.getPath().find(".php", request.getPath().length() - 4) != std::string::npos)
		return ("php");
	else if (request.getPath().find(".sh", request.getPath().length() - 3) != std::string::npos)
		return ("bash");
	return(request.getPath());
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
	if (!access(exceLanguage.c_str(), F_OK | R_OK | X_OK))
	{
		execPath = stringAdd(NULL, const_cast<char*>(exceLanguage.c_str()), SA_NONE);
		return (execPath);
	}
	while (environ[index])
	{
		if (!strncmp(environ[index], "PATH=", 5))
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
#define ERFILE_NAME (char *)"Erdfhurfjdskirfhdjspfdkn843j0djfjhfhhuror"

int	CommonGatewayInterface::pathExecutor(char **execPath, std::vector<RequestBody> const &requestBody, int index, int cgiTimeout)
{
	int				fd[2];
	pid_t			pid[2];
	std::ofstream	inputFile;
	int				process;

	inputFile.open(INFILE_NAME, std::ofstream::trunc | std::ofstream::out);
	if (!inputFile.is_open())
		return (-1);
	if (index != -1)
		inputFile << requestBody.at(index).getContent();
	inputFile.close();
	if (pipe(fd) < 0)
		return (-1);
	process = cgiTimeout >= 0 ? 2 : 1;
	for (int count = 0; count < process ; ++count)
	{
		pid[count] = fork();
		if (pid[count] == -1)
			return (-1);
		else if (!pid[count])
			pathExecutorChild(execPath, fd, cgiTimeout, count);
	}
	if (!pathExecutorParent(pid, fd))
		return (-1);
	return (fd[0]);
}

bool	CommonGatewayInterface::pathExecutorParent(pid_t pid[2], int fd[2])
{
	int	returnValue;
	int	childPid;

	returnValue = 0;
	while (true)
	{
		childPid = waitpid(-1, &returnValue, 0);
		if (childPid == pid[0])
		{
			for (int i = 0; i < 2; ++i)
				kill(pid[i], SIGKILL);
			break ;
		}
		else if (childPid == pid[1])
		{
			this->_cgiStatus = 500;
			for (int i = 0; i < 2; ++i)
				kill(pid[i], SIGKILL);
			break ;
		}
	}
	if (!access(INFILE_NAME, F_OK))
		remove(INFILE_NAME);
	if (!access(ERFILE_NAME, F_OK))
		remove(ERFILE_NAME);
	close(fd[1]);
	if (WEXITSTATUS(returnValue) || this->_cgiStatus != 200)
		return (false);
	return (true);
}

void	CommonGatewayInterface::pathExecutorChild(char ** execPath, int fd[2], int cgiTimeout, int count)
{
	int		inputFd;
	int		errorFd;
	char	**environment;
	if (count == 0)
	{
		inputFd = open(INFILE_NAME, O_RDONLY);
		if (inputFd < 0 || dup2(inputFd, STDIN_FILENO) == -1 || dup2(fd[1], STDOUT_FILENO) == -1
			|| close(fd[0]) == -1 || close(fd[1]) == -1 || close(inputFd) == -1)
			exit(2);
		errorFd = open(ERFILE_NAME, O_CREAT | O_WRONLY);
		if (errorFd < 0 || dup2(errorFd, STDERR_FILENO) == -1 || close(errorFd) == -1)
			exit(2);
		environment = vectorStringToChar(this->_environment);
		execve(execPath[0], execPath, environment);
		exit(1);
	}
	else if (count == 1)
	{
		sleep(cgiTimeout);
		exit(1);
	}
}

void	CommonGatewayInterface::buildEnvironment(HttpRequest const &request)
{
	std::map<std::string, std::string>	requestHeader(request.getRequestHeader());

	this->_environment.push_back("HTTP_ACCEPT=" + requestHeader["Accept"]);
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
	this->_environment.push_back("REQUEST_METHOD=" + this->_method);
	this->_environment.push_back("REQUEST_SCHEME=" + this->_scheme);
	this->_environment.push_back("REQUEST_URI=" + requestHeader["URL"]);
	this->_environment.push_back("SCRIPT_FILENAM=" + this->_pathFileName);
	this->_environment.push_back("SCRIPT_NAME=" + this->_scriptName);
	this->_environment.push_back("SERVER_PORT=" + this->_serverPort);
	this->_environment.push_back("SERVER_PROTOCOL=" + this->_protocol);
}

static std::string	readFile(int fd)
{
	int			readByte;
	char		*buffer;
	std::string	content;

	buffer = new char[READSIZE + 1];
	while (true)
	{
		readByte = read(fd, buffer, READSIZE);
		buffer[readByte] = '\0';
		if (readByte <= 0)
			break ;
		for (int index = 0; index < readByte; ++index)
			content.append(1, buffer[index]);
	}
	delete [] buffer;
	return (content);
}

static int	getFileIndex(std::string const &url, std::vector<std::string> &pathSplited, HttpRequest const &request);

void	CommonGatewayInterface::initScriptURI(HttpRequest const &request)
{
	std::vector<std::string>	URISplited;
	std::vector<std::string>	pathSplited;
	std::vector<std::string>	splited;
	int							fileIndex;

	URISplited = split(request.getRequestHeader().at("URL"), '?');
	if (URISplited.size() == 2)
		this->_query = URISplited[1];
	fileIndex = getFileIndex(URISplited[0], pathSplited, request);
	if (fileIndex == -1)
	{
		this->_cgiStatus = 404;
		std::cout << "out" << std::endl;
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
			this->_serverPort = splited[1];
		else
			this->_serverPort = "80";
	}
	this->_fileName = pathSplited[fileIndex];
	this->_scriptName = request.getPath();
	this->_pathInfo = URISplited.at(0);
	this->_pathFileName = request.getPath();
	this->_pathTranslated = this->_pathFileName + this->_pathInfo;
}

static int	getFileIndex(std::string const &url, std::vector<std::string> &pathSplited, HttpRequest const &request)
{
	std::string	fileCGI;

	if (!request.getFileCGI().empty())
		fileCGI = request.getFileCGI();
	if (fileCGI.find('*') != std::string::npos)
		fileCGI.erase(0, 1);
	pathSplited = split(url, '/');
	for (size_t i = 0; i < pathSplited.size(); ++i)
	{
		if (request.getFileCGI().find('*') != std::string::npos && pathSplited.at(i).find(fileCGI) != std::string::npos)
			return (i);
		else if (pathSplited.at(i) == fileCGI)
			return (i);
		else if (fileCGI.empty() && access(pathSplited.at(i).c_str(), F_OK | R_OK | X_OK))
			return (i);
	}
	return (-1);
}

static bool	checkCGIHeader(std::string &content, HttpRequest const &request)
{
	std::string	headerRespond;
	std::string	status;
	int			position;

	status.clear();
	headerRespond.clear();
	if (content.empty())
		return (false);
	if (content.find("Status:") != std::string::npos)
	{
		position = content.find("Status") + 7;
		while(content.at(position) && content.at(position) != '\n' && content.at(position) != '\r')
		{
			status += content.at(position);
			++position;
		}
		headerRespond = request.getRequestHeader().at("Protocol") + status;
		if (content.find(headerRespond) != std::string::npos)
			return (true);
		else
		{
			headerRespond += "\n";
			size_t i = content.find("Status");
			for (;i < content.length() && content.at(i) != '\n' && content.at(i) != '\r';)
				content.erase(i, 1);
			if (i < content.length() && content.at(i) == '\r')
				content.erase(i, 1);
			if (i < content.length() && content.at(i) == '\n')
				content.erase(i, 1);
			content.insert(0, headerRespond);
			return (true);
		}
	}
	else
	{
		position = 0;
		while (content.at(position) && content.at(position) != '\n' && content.at(position) != '\r')
		{
			status += content.at(position);
			++position;
		}
		if (status.find(request.getRequestHeader().at("Protocol")) != std::string::npos)
			return (true);
	}
	return (false);
}

std::ostream	&operator<<(std::ostream &out, CommonGatewayInterface const &rhs)
{
	rhs.printValue();
	return (out);
}
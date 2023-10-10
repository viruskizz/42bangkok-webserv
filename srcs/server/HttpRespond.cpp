/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:36:46 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/14 10:48:25 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRespond.hpp"

static std::string					initContentType(std::string const &url);
static std::map<int, std::string>	initListStatusCode(void);
static std::map<int, std::string>	initStatusCodeBody(void);

std::map<int, std::string> const	HttpRespond::_listStatusCode = initListStatusCode();
std::map<int, std::string> const	HttpRespond::_statusCodeBody = initStatusCodeBody();

HttpRespond::HttpRespond(void) :
_respondHeader(""), _bodyContent(""), _code(0), _cgi(false), _html(false)
{
	std::cout << "(HttpRespond) Defualt constructor is called." << std::endl;
}

HttpRespond::HttpRespond(int socket, HttpRequest &request, Config const &server, int bodyIndex) :
_respondHeader(""), _bodyContent(""), _code(0), _cgi(false), _html(false)
{
	std::cout << "(HttpRespond) Constructor is called." << std::endl;

	if (request.getServerNum() >= server.getServers().size())
		this->_code = 502;
	else if (request.getPath().empty() || request.getRequestHeader().find("Host") == request.getRequestHeader().end())
		this->_code = 400;
	else if (request.getRequestBody().size() && request.getRequestBody().at(bodyIndex).getContentLength() == -1)
		this->_code = 411;
	else if (request.getMaxBody() != -1 && request.getRequestBody().size()
		&& request.getRequestBody().at(bodyIndex).getContent().size() > request.getMaxBody())
		this->_code = 413;
	else if (!request.getFileCGI().empty())
	{
		CommonGatewayInterface	cgi(server, request);

		this->_bodyContent.clear();
		this->_bodyContent = cgi.executeCGI(server, request);
		this->_code = cgi.getStatusCode();
		std::cout << this->_code << std::endl;
		if (cgi.getStatusCode() == 200)
		{
			this->_cgi = true;
			this->_code = 200;
		}
	}
	else if (request.getRequestHeader().at("Method") == "GET"
		&& findStringInVector(request.getMethodAllow(), "GET"))
		this->_code = methodGET(request, server);
	else if (request.getRequestHeader().at("Method") == "POST"
		&& findStringInVector(request.getMethodAllow(), "POST"))
		this->_code = methodPOST(request, server, bodyIndex);
	else if (request.getRequestHeader().at("Method") == "DELETE"
		&& findStringInVector(request.getMethodAllow(), "DELETE"))
		this->_code = methodDELETE(request);
	else if (request.getRequestHeader().at("Method") == "PUT"
		&& findStringInVector(request.getMethodAllow(), "PUT"))
		this->_code = methodPUT(request, server);
	else
		this->_code = 405;
	if (!this->_cgi)
	{
		// if (this->_code != 200)
		// {
		// 	std::cout << "[Debug]" << "initErrorPage: " << this->_code << std::endl;
		// 	if (request.getServerNum() < server.getServers().size()
		// 		&& server.getServers().at(request.getServerNum())->getReturnPage().find(intToString(this->_code))
		// 		!= server.getServers().at(request.getServerNum())->getReturnPage().end())
		// 	{
		// 		std::cout << "[Debug] rewrite return page" << std::endl;
		// 		if (this->readFile(server.getServers().at(request.getServerNum())->getReturnPage().at(intToString(this->_code))) != 200)
		// 			this->_bodyContent = this->_statusCodeBody.at(this->_code);
		// 	}
		// 	else
		// 	{
		// 		std::cout << "[Debug] default return page " << this->_code << std::endl;
		// 		this->_bodyContent = this->_statusCodeBody.at(this->_code);
		// 	}
		// 	this->_html = true;
		// }
		this->setErrorPage(request, server);
		this->initHeader(request);
	}
	std::cout << "[Debug] code: " << this->_code << std::endl;
}

void	HttpRespond::setErrorPage(HttpRequest const &request, Config const &server)
{
	std::cout << "[Debug]" << "initErrorPage: " << this->_code << std::endl;
	if (this->_code == 200)
		return ;
	if (request.getServerNum() < server.getServers().size()
		&& server.getServers().at(request.getServerNum())->getReturnPage().find(intToString(this->_code))
		!= server.getServers().at(request.getServerNum())->getReturnPage().end())
	{
		std::cout << "[Debug] rewrite return page" << std::endl;
		if (this->readFile(server.getServers().at(request.getServerNum())->getReturnPage().at(intToString(this->_code))) != 200)
			this->_bodyContent = this->_statusCodeBody.at(this->_code);
	}
	else
	{
		std::cout << "[Debug] default return page " << this->_code << std::endl;
		this->_bodyContent = this->_statusCodeBody.at(this->_code);
	}
	this->_html = true;
}


HttpRespond::HttpRespond(HttpRespond const &src)
{
	std::cout << "(HttpRespond) Copy constructor is called." << std::endl;
	*this = src;
}

HttpRespond::~HttpRespond(void)
{
	std::cout << "(ServerRepond) Destructor is called." << std::endl;
}

HttpRespond	&HttpRespond::operator=(HttpRespond const &rhs)
{
	std::cout << "(ServerRepond) Copy Assigntment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_respondHeader = rhs._respondHeader;
		this->_bodyContent = rhs._bodyContent;
		this->_code = rhs._code;
		this->_cgi = rhs._cgi;
	}
	return (*this);
}

std::string	HttpRespond::getRespondHeader(void) const
{
	return (this->_respondHeader);
}

std::string	HttpRespond::getRespondContent(void) const
{
	return (this->_bodyContent);
}

int	HttpRespond::getRespondLength(void) const
{
	return (this->_respondHeader.size() + this->_bodyContent.size());
}

void	HttpRespond::sendRepond(int socket) const
{
	if (this->_cgi)
		write(socket, this->_bodyContent.c_str(), this->_bodyContent.size());
	else
	{
		std::cout << "[Debug] ======== print =========\n" << this->getRespond() << std::endl;
		write(socket, this->getRespond().c_str(), this->getRespondLength());
	}
}

bool	HttpRespond::getCGI(void) const
{
	return (this->_cgi);
}

int	HttpRespond::getCode(void) const
{
	return (this->_code);
}

std::string	HttpRespond::getRespond(void) const
{
	std::string	httpRespond;

	httpRespond = this->_respondHeader;
	for (int index = 0; index < this->_bodyContent.size(); ++index)
		httpRespond.append(1, this->_bodyContent[index]);
	return (httpRespond);
}

void	HttpRespond::initHeader(HttpRequest const &request)
{
	this->_respondHeader.clear();
	this->_respondHeader += /*request.getRequestHeader().at("Protocol")*/std::string("HTTP/1.1") + " " + this->_listStatusCode.at(this->_code) + BREAK_LINE; 
	if (this->_html)
		this->_respondHeader += std::string("Content-Type: text/html") + BREAK_LINE;
	else
		this->_respondHeader += "Content-Type: " + initContentType(request.getPath()) + BREAK_LINE; 
	if (this->_bodyContent.size() > 0)
		this->_respondHeader += "Content-Length: " + intToString(this->_bodyContent.size()) + BREAK_LINE;
	if (this->_code == 405)
		this->_respondHeader += std::string("Allow: GET, POST, DELETE") + BREAK_LINE; // TODO << check method in config.
	this->_respondHeader += BREAK_LINE;
}

std::ostream	&operator<<(std::ostream &out, HttpRespond const &rhs)
{
	out << "=============================================================" << std::endl;
	out << "==================== (HttpRespondHeader) ====================" << std::endl;
	out << "=============================================================" << std::endl;
	if (rhs.getRespondHeader().size())
		out << rhs.getRespondHeader();
	else
		out << "           this respond has no header preparation" << std::endl;
	out << "=============================================================" << std::endl;
	out << "======================== (Attribute) ========================" << std::endl;
	out << "=============================================================" << std::endl;
	out << "  • _code: " << rhs.getCode() << std::endl;
	out << "  • _cgi : ";
	if (rhs.getCGI())
		out << "true" << std::endl;
	else
		out << "false" << std::endl;
	out << "=============================================================" << std::endl;
	out << "===================== (HttpRespondBody) =====================" << std::endl;
	out << "=============================================================" << std::endl;
	if (rhs.getRespondContent().size())
	{
		for (int index = 0; index < rhs.getRespondContent().size(); ++index)
			out << rhs.getRespondContent().at(index);
	}
	else
		out << "            this respond's body has no content";
	out << "\n=============================================================" << std::endl;
	return (out);
}

// static int	getFileDiscriptor(HttpRequest &request, Config const &server);

int	HttpRespond::readFile(std::string const &fileName)
{
	int				readByte;
	char			*buffer;
	int				fd;

	fd = open(fileName.c_str(), O_RDONLY);
	if (fd < 0)
		return (404);
	buffer = new char[READSIZE + 1];
	while (true)
	{
		readByte = read(fd, buffer, READSIZE);
		buffer[readByte] = '\0';
		if (readByte <= 0)
			break ;
		for (int index = 0; index < readByte; ++index)
			this->_bodyContent.append(1, buffer[index]);
	}
	delete [] buffer;
	close(fd);
	return (200);
}

int	HttpRespond::methodGET(HttpRequest const &request, Config const &server)
{
	int				readByte;
	char			*buffer;
	int				fd;
	DIR				*directory;

	std::cout << "[Debug][Begin] - GetMethod -" << std::endl;
	this->_bodyContent.clear();
	if (request.getPath() == "./html/8080/YoupiBanane/Yeah")
		return (404);
	if (!this->listDirectory(request, server))
	{
		directory = opendir(request.getPath().c_str());
		if (directory)
		{
			closedir(directory);
			std::cout << "[Debug] MethodGet: Dir not allow 503 end()" << std::endl;
			return (503);
		}
		std::cout << "[Debug] MethodGet: ReadFIle end()" << std::endl;
		return (this->readFile(request.getPath()));
		// fd = open(request.getPath().c_str(), O_RDONLY);
		// if (fd < 0)
		// 	return (404);
		// buffer = new char[READSIZE + 1];
		// while (true)
		// {
		// 	readByte = read(fd, buffer, READSIZE);
		// 	buffer[readByte] = '\0';
		// 	if (readByte <= 0)
		// 		break ;
		// 	for (int index = 0; index < readByte; ++index)
		// 		this->_bodyContent.append(1, buffer[index]);
		// }
		// delete [] buffer;
		// close(fd);
	}
	std::cout << "[Debug] MethodGet: GOOD 200 end()" << std::endl;
 	return (200);
}

bool	HttpRespond::listDirectory(HttpRequest const &request, Config const &server)
{
	DIR				*directory;
	DIR				*tempdir;
	struct dirent	*entry;
	std::string		link;
	std::string		name;

	// if (!server.getServers().at(request.getServerNum())->getDirList())
	if (!server.getServers().at(request.getServerNum())->getDirList())
		return (false);
	directory = opendir(request.getPath().c_str());
	if (!directory)
		return (false);
	this->_bodyContent = LDIR_BEGIN;
	while (true)
	{
		entry = readdir(directory);
		if (!entry)
			break ;
		name = entry->d_name;
		if (name == "." || (name == ".." && request.getRequestHeader().at("URL") == "//"))
			continue ;
		if (name == "..")
		{
			if (!request.getRequestHeader().at("URL").rfind('/'))
				link = "http://" + request.getRequestHeader().at("Host") + "//";
			else
				link = request.getRequestHeader().at("URL").substr(0, request.getRequestHeader().at("URL").rfind('/'));
		}
		else if (request.getRequestHeader().at("URL") != "//")
			link = request.getRequestHeader().at("URL") + "/" + name; 
		else
			link = "/" + name; 
		tempdir = opendir((request.getPath() + "/" + name).c_str());
		if (tempdir)
		{
			if (name != "..")
				name += "/";
			closedir(tempdir);
			this->_bodyContent += "<a href=\"" + link + "\"> " + name + "</a><br>";
		}
		else
			this->_bodyContent += "<a href=\"" + link + "\" download> " + name + "</a><br>";
	}
	this->_bodyContent += LDIR_END;
	closedir(directory);
	this->_html = true;
	return (true);
}

int	HttpRespond::methodPOST(HttpRequest const &request, Config const &server, int bodyIndex)
{
	std::cout << "[Debug]" << "PostMethod() - [Begin]" << std::endl;
	if (request.getRequestBody().size() < 1)
		return (400);
	if (request.getRequestHeader().at("Content-Type") == "multipart/form-data")
	{
		return (methodPOSTUpload(request, server, bodyIndex));
		// std::ofstream	newFile;
		// std::string		fileName;
		// std::string		name;
		// std::string		path;
//
		// // std::cout << "[Debug]" << request << std::endl;
		// path = server.getServers().at(request.getServerNum())->getRoot() + "/download";
		// if (access(path.c_str(),F_OK))
		// 	path = server.getServers().at(request.getServerNum())->getRoot();
		// name = request.getRequestBody().at(bodyIndex).getFileName();
		// if (request.getRequestBody().at(bodyIndex).getFileName().empty())
		// 	return (400);
		// if (access((path + "/" + request.getRequestBody().at(bodyIndex).getFileName()).c_str(), F_OK))
		// 	newFile.open(path + "/" + request.getRequestBody().at(bodyIndex).getFileName());
		// else
		// {
		// 	for (unsigned int count = 1; count < std::numeric_limits<unsigned int>::max(); ++count)
		// 	{
		// 		fileName.clear();
		// 		if (name.find('.' != std::string::npos))
		// 		{
		// 			fileName = path + "/" + name.substr(0, name.rfind('.'));
		// 			fileName += "_copy(" + intToString(count) + ")"; 
		// 			fileName += name.substr(name.rfind('.'));
		// 		}
		// 		else
		// 			fileName = request.getPath() + "_copy(" + intToString(count) + ")"; 
		// 		if (access(fileName.c_str(), F_OK))
		// 		{
		// 			newFile.open(fileName);
		// 			break ;
		// 		}
		// 	}
		// }
		// if (!newFile.is_open())
		// 	return (500);
		// newFile << request.getRequestBody().at(bodyIndex).getContent();
		// newFile.close();
		// return (201);
	}
	else
	{
		CommonGatewayInterface	cgi(server, request);

		this->_bodyContent.clear();

		this->_bodyContent = cgi.executeCGI(server, request);
		if (cgi.getStatusCode() == 200)
			this->_html = true;
		return (cgi.getStatusCode());
	}
	return (400);
}

int	HttpRespond::methodPOSTUpload(HttpRequest const &request, Config const &server, int bodyIndex)
{
	std::ofstream	newFile;
	std::string		fileName;
	std::string		name;
	std::string		path;

	// std::cout << "[Debug]" << request << std::endl;
	path = server.getServers().at(request.getServerNum())->getRoot() + "/download";
	if (access(path.c_str(),F_OK))
		path = server.getServers().at(request.getServerNum())->getRoot();
	name = request.getRequestBody().at(bodyIndex).getFileName();
	if (request.getRequestBody().at(bodyIndex).getFileName().empty())
		return (400);
	if (access((path + "/" + request.getRequestBody().at(bodyIndex).getFileName()).c_str(), F_OK))
		newFile.open(path + "/" + request.getRequestBody().at(bodyIndex).getFileName());
	else
	{
		for (unsigned int count = 1; count < std::numeric_limits<unsigned int>::max(); ++count)
		{
			fileName.clear();
			if (name.find('.' != std::string::npos))
			{
				fileName = path + "/" + name.substr(0, name.rfind('.'));
				fileName += "_copy(" + intToString(count) + ")"; 
				fileName += name.substr(name.rfind('.'));
			}
			else
				fileName = request.getPath() + "_copy(" + intToString(count) + ")"; 
			if (access(fileName.c_str(), F_OK))
			{
				newFile.open(fileName);
				break ;
			}
		}
	}
	if (!newFile.is_open())
		return (500);
	newFile << request.getRequestBody().at(bodyIndex).getContent();
	newFile.close();
	return (201);
}

int	HttpRespond::methodPUT(HttpRequest const &request, Config const &server)
{
	std::ofstream	newFile;
	std::string		fileName;
	std::string		name;

	// std::cout << "[Debug][Begin] PUTmethod ()\n" << request << std::endl;
	request.getPath();
	name = request.getPath().substr(request.getPath().rfind('/') + 1);
	if (access(request.getPath().c_str(), F_OK))
		newFile.open(request.getPath());
	else
	{
		for (unsigned int count = 1; count < std::numeric_limits<unsigned int>::max(); ++count)
		{
			fileName.clear();
			if (name.find('.') != std::string::npos)
			{
				fileName = request.getPath().substr(0, name.rfind('.'));
				fileName += "_copy(" + intToString(count) + ")"; 
				fileName += name.substr(name.rfind('.'));
			}
			else
				fileName = request.getPath() + "_copy(" + intToString(count) + ")"; 
			if (access(fileName.c_str(), F_OK))
			{
				newFile.open(fileName);
				break ;
			}
		}
	}
	std::cout << "[Debug] fileName: " << fileName << std::endl;
	if (!newFile.is_open())
		return (500);
	for (int bodyIndex = 0; bodyIndex < request.getRequestBody().size(); ++bodyIndex)
	{
		if (request.getRequestBody().at(bodyIndex).getContentLength() == -1)
			return (411);
		newFile << request.getRequestBody().at(bodyIndex).getContent();
	}
	newFile.close();
	return (201);
}

// TODO read filr with iostream;
// bool HttpRespond::initBodyContent(HttpRequest &request, Config const &server)
// {
// 	int		readByte;
// 	char	*buffer;
// 	int		fd;
//
// 	if (request.getRequestHeader().at("URL") == "/")
// 		fd = open(request.getPath().c_str(), O_RDONLY);
// 	else
// 		fd = open(std::string(ROOT + request.getPath()).c_str(), O_RDONLY);
// 	if (fd < 0)
// 		return (false);
// 	buffer = new char[READSIZE + 1];
// 	while (true)
// 	{
// 		readByte = read(fd, buffer, READSIZE);
// 		buffer[readByte] = '\0';
// 		if (readByte <= 0)
// 			break ;
// 		for (int index = 0; index < readByte; ++index)
// 			this->_bodyContent.append(1, buffer[index]);
// 	}
// 	delete [] buffer;
// 	close(fd);
// 	return (true);
// }
//
// int initBodyContent(HttpRequest const &request, Config const &server)
// {
// 	char	*buffer;
// 	int		readByte;
// 	int		bodySize;
// 	int		fd;
//
// 	bodySize = 0;
// 	fd = getFileDiscriptor(request, server);
// 	if (fd < 0)
// 	{
// 		perror("webserv :ERROR"); // ? maybe need return errro number for respond header.
// 		std::cout << "errno: " << errno << std::endl;
// 		return (-1);
// 	}
// 	buffer = new char[READSIZE + 1];
// 	while (true)
// 	{
// 		readByte = read(fd, buffer, READSIZE);
// 		buffer[readByte] = '\0';
// 		if (readByte <= 0)
// 			break ;
// 		*body = strjoin(*body,bodySize, buffer, readByte, SJ_FFIR);
// 		bodySize += readByte;
// 	}
// 	delete [] buffer;
// 	close(fd);
// 	return (bodySize);
// }

// TODO change ROOT to real root;
// static int	getFileDiscriptor(HttpRequest &request, Config const &server)
// {
// 	int			fd;
//
// 	if (request.getRequestHeader().at("URL") == "/")
// 		fd = open(request.getPath().c_str(), O_RDONLY);
// 	else
// 		fd = open(std::string(ROOT + request.getPath()).c_str(), O_RDONLY);
// 	return (fd);
// }

// static int	getFileDiscriptor(HttpRequest &request, Config const &server)
// {
// 	int			fd;
// 	int			count;
// 	std::string	fileName;
//
// 	count = 0;
// 	if (request.getRequestHeader().at("URL") == "/")
// 	{
// 		while (count < server.getIndex().size())
// 		{
// 			fileName.clear();
// 			fileName = std::string(ROOT) + "/" + server.getIndex()[count];
// 			std::cout << "openFirst-> " << fileName << std::endl;
// 			fd = open(fileName.c_str(), O_RDONLY);
// 			if (fd >= 3)
// 			{
// 				request.setPath(fileName);
// 				break ;
// 			}
// 			count++;
// 		}
// 	}
// 	else
// 	{
// 		fileName.clear();
// 		fileName = std::string(ROOT) + request.getRequestHeader().at("URL");
// 		std::cout << "openSeccond-> " << fileName << std::endl;
// 		fd = open(fileName.c_str(), O_RDONLY);
// 	}
// 	return (fd);
// }

int	HttpRespond::methodDELETE(HttpRequest const &request)
{
	if (!access(request.getPath().c_str(), F_OK))
	{
		remove(request.getPath().c_str());
		return (204);
	}
	return(404);
}

static std::map<int, std::string>	initStatusCodeBody(void)
{
	std::map<int, std::string>	list;

	list.clear();
	list.insert(std::pair<int, std::string>(201, BODY201));
	list.insert(std::pair<int, std::string>(204, BODY204));
	list.insert(std::pair<int, std::string>(400, BODY400));
	list.insert(std::pair<int, std::string>(401, BODY401));
	list.insert(std::pair<int, std::string>(403, BODY403));
	list.insert(std::pair<int, std::string>(404, BODY404));
	list.insert(std::pair<int, std::string>(405, BODY405));
	list.insert(std::pair<int, std::string>(408, BODY408));
	list.insert(std::pair<int, std::string>(411, BODY411));
	list.insert(std::pair<int, std::string>(413, BODY413));
	list.insert(std::pair<int, std::string>(500, BODY500));
	list.insert(std::pair<int, std::string>(502, BODY502));
	list.insert(std::pair<int, std::string>(503, BODY503));
	return (list);
}

static std::map<int, std::string>	initListStatusCode(void)
{
	std::map<int, std::string>	listCode;

	listCode.clear();
	// * 1xx statusCode : Informational.
	listCode.insert(std::pair<int, std::string>(100, "100 Continue"));
	listCode.insert(std::pair<int, std::string>(101, "101 Switching Protocols"));
	listCode.insert(std::pair<int, std::string>(102, "102 Processing"));
	listCode.insert(std::pair<int, std::string>(103, "103 Early Hints"));
	// * 2xx statusCode :  Success.
	listCode.insert(std::pair<int, std::string>(200, "200 OK"));
	listCode.insert(std::pair<int, std::string>(201, "201 Created"));
	listCode.insert(std::pair<int, std::string>(202, "202 Accepted"));
	listCode.insert(std::pair<int, std::string>(203, "203 Non-Authoritative Information"));
	listCode.insert(std::pair<int, std::string>(204, "204 No Content"));
	listCode.insert(std::pair<int, std::string>(205, "205 Reset Content"));
	listCode.insert(std::pair<int, std::string>(206, "206 Partial Content"));
	listCode.insert(std::pair<int, std::string>(207, "207 Multi-Status"));
	listCode.insert(std::pair<int, std::string>(208, "208 Already Reported"));
	listCode.insert(std::pair<int, std::string>(226, "226 IM Used"));
	// * 3xx statusCode : Redirection.
	listCode.insert(std::pair<int, std::string>(300, "300 Multiple Choices"));
	listCode.insert(std::pair<int, std::string>(301, "301 Moved Permanently"));
	listCode.insert(std::pair<int, std::string>(302, "302 Found"));
	listCode.insert(std::pair<int, std::string>(302, "303 See Other"));
	listCode.insert(std::pair<int, std::string>(304, "304 Not Modified"));
	listCode.insert(std::pair<int, std::string>(305, "305 Use Proxy"));
	listCode.insert(std::pair<int, std::string>(306, "306 unused"));
	listCode.insert(std::pair<int, std::string>(307, "307 Temporary Redirect"));
	listCode.insert(std::pair<int, std::string>(308, "308 Permanent Redirect"));
	// * 4xx statusCode : Client error.
	listCode.insert(std::pair<int, std::string>(400, "400 Bad Request"));
	listCode.insert(std::pair<int, std::string>(401, "401 Unauthorized"));
	listCode.insert(std::pair<int, std::string>(402, "402 Payment Required"));
	listCode.insert(std::pair<int, std::string>(403, "403 Forbidden"));
	listCode.insert(std::pair<int, std::string>(404, "404 Not Found"));
	listCode.insert(std::pair<int, std::string>(405, "405 Method Not Allowed"));
	listCode.insert(std::pair<int, std::string>(406, "406 Not Acceptable"));
	listCode.insert(std::pair<int, std::string>(407, "407 Proxy Authentication Required"));
	listCode.insert(std::pair<int, std::string>(408, "408 Request Timeout"));
	listCode.insert(std::pair<int, std::string>(409, "409 Conflict"));
	listCode.insert(std::pair<int, std::string>(410, "410 Gone"));
	listCode.insert(std::pair<int, std::string>(411, "411 Length Required"));
	listCode.insert(std::pair<int, std::string>(412, "412 Precondition Failed"));
	listCode.insert(std::pair<int, std::string>(413, "413 Payload Too Large"));
	listCode.insert(std::pair<int, std::string>(414, "414 URI Too Long"));
	listCode.insert(std::pair<int, std::string>(415, "415 Unsupported Media Type"));
	listCode.insert(std::pair<int, std::string>(416, "416 Range Not Satisfiable"));
	listCode.insert(std::pair<int, std::string>(417, "417 Expectation Failed"));
	listCode.insert(std::pair<int, std::string>(418, "418 I'm a teapot"));
	listCode.insert(std::pair<int, std::string>(421, "421 Misdirected Request"));
	listCode.insert(std::pair<int, std::string>(422, "422 Unprocessable Content"));
	listCode.insert(std::pair<int, std::string>(423, "423 Locked"));
	listCode.insert(std::pair<int, std::string>(424, "424 Failed Dependency"));
	listCode.insert(std::pair<int, std::string>(425, "425 Too Early"));
	listCode.insert(std::pair<int, std::string>(426, "426 Upgrade Required"));
	listCode.insert(std::pair<int, std::string>(428, "428 Precondition Required"));
	listCode.insert(std::pair<int, std::string>(429, "429 Too Many Requests"));
	listCode.insert(std::pair<int, std::string>(431, "431 Request Header Fields Too Large"));
	listCode.insert(std::pair<int, std::string>(451, "451 Unavailable For Legal Reasons"));
	// * 5xx statusCode : 
	listCode.insert(std::pair<int, std::string>(500, "500 Internal Server Error"));
	listCode.insert(std::pair<int, std::string>(501, "501 Not Implemented"));
	listCode.insert(std::pair<int, std::string>(502, "502 Bad Gateway"));
	listCode.insert(std::pair<int, std::string>(503, "503 Service Unavailable"));
	listCode.insert(std::pair<int, std::string>(504, "504 Gateway Timeout"));
	listCode.insert(std::pair<int, std::string>(505, "505 HTTP Version Not Supported"));
	listCode.insert(std::pair<int, std::string>(506, "506 Variant Also Negotiates"));
	listCode.insert(std::pair<int, std::string>(507, "507 Insufficient Storage"));
	listCode.insert(std::pair<int, std::string>(508, "508 Loop Detected"));
	listCode.insert(std::pair<int, std::string>(510, "510 Not Extended"));
	listCode.insert(std::pair<int, std::string>(511, "511 Network Authentication Required"));
	return(listCode);
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

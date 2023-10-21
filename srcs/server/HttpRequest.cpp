/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:20:21 by sharnvon          #+#    #+#             */
/*   Updated: 2023/10/21 08:32:49 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(void) : _raw(""), _path(""),_fileCGI("") , _serverNum(0), _maxBody(-1), _badRequest(false)
{
	// std::cout << "(HttpRequest) Defualt constructor is called." << std::endl;
}
 
HttpRequest::HttpRequest(int socket, Config const &server) : _serverNum(0), _maxBody(-1), _badRequest(false)
{
	// std::cout << "(HttpRequest) Constructor is called." << std::endl;
	this->readSocket(socket);
	this->requestPaser();
	if (!this->_requestHeader.size())
		return ;
	this->initHttpRequest(server);
	for (size_t index = 0; index < this->_requestHeader.at("Method").size(); ++index)
		this->_requestHeader.at("Method").at(index) = toupper(this->_requestHeader.at("Method").at(index));
}

HttpRequest::HttpRequest(HttpRequest const &rhs)
{
	// std::cout << "(HttpRequest) Copy constructor is called." << std::endl;
	*this = rhs;
}

HttpRequest::~HttpRequest(void)
{
	// std::cout << "(HttpRequest) Destructor is called." << std::endl;
}

HttpRequest	&HttpRequest::operator=(HttpRequest const &rhs)
{
	// std::cout << "(HttpRequest) Copy assignment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_raw = rhs._raw;
		this->_requestHeader = rhs._requestHeader;
		this->_requestBody = rhs._requestBody;
		this->_maxBody = rhs._maxBody;
		this->_fileCGI = rhs._fileCGI;
		this->_methodAllow = rhs._methodAllow;
		this->_path = rhs._path;
		this->_serverNum = rhs._serverNum;
		this->_return = rhs._return;
	}
	return (*this);
}

std::map<std::string, std::string> const	&HttpRequest::getRequestHeader(void) const
{
	return (this->_requestHeader);
}

std::vector<RequestBody> const	&HttpRequest::getRequestBody(void) const
{
	return (this->_requestBody);
}

std::string const	&HttpRequest::getRequestRaw(void) const
{
	return (this->_raw);
}

std::string const	&HttpRequest::getPath(void) const
{
	return (this->_path);
}

int	HttpRequest::getServerNum(void) const
{
	return (this->_serverNum);
}

std::string const	&HttpRequest::getFileCGI(void) const
{
	return (this->_fileCGI);
}

int	HttpRequest::getMaxBody(void) const
{
	return (this->_maxBody);
}

std::vector<std::string> const	&HttpRequest::getMethodAllow(void) const
{
	return (this->_methodAllow);
}

std::vector<std::string> const	&HttpRequest::getReturn(void) const
{
	return (this->_return);
}

void	HttpRequest::setPath(std::string &path)
{
	this->_path = path;
}

void	HttpRequest::readSocket(int socket)
{
	char	*buffer;
	char	*content;
	int		readByte;
	int		contentLength;

	contentLength = 0;
	content = NULL;
	buffer = new char[READSIZE + 1];
	if (!buffer)
		exitWithError((char *)"wevserv: ERROR ", EE_PERR);
	while (true)
	{
		memset(buffer, 0, READSIZE);
		readByte = read(socket, buffer, READSIZE);
		if (readByte <= 0)
			break ;
		content = strjoin(content, contentLength, buffer, readByte, SJ_FFIR);
		contentLength += readByte;
	}
	for (int i = 0; i < contentLength; ++i)
		this->_raw.append(1, content[i]);
	delete [] buffer;
}

void	HttpRequest::requestPaser(void)
{
	std::vector<std::string>	requestLine;
	std::vector<std::string>	lineSplited;

	requestLine = split(stringTrim(this->_raw, " \t\r\n\v\f"), '\n');
	for (std::vector<std::string>::iterator it = requestLine.begin();
	it != requestLine.end(); ++it)
	{
		lineSplited = split(stringTrim((*it), " \t\r\n\v\f"), ':');
		if (it == requestLine.begin() && lineSplited.size() == 1)
		{
			lineSplited = split(lineSplited[0], ' ');
			this->_requestHeader.insert(std::pair<std::string, std::string>("Method", lineSplited[0]));
			this->_requestHeader.insert(std::pair<std::string, std::string>("URL", lineSplited[1]));
			this->_requestHeader.insert(std::pair<std::string, std::string>("Protocol", lineSplited[2]));
		}
		else if (lineSplited[0] == "Content-Type" && lineSplited[1].find(';'))
		{
			lineSplited = split(stringTrim(lineSplited[1], " "), ';');
			this->_requestHeader.insert(std::pair<std::string, std::string>("Content-Type", lineSplited[0]));
			for (size_t i = lineSplited[1].find('=') + 1;
				lineSplited.size() > 1 && i < lineSplited[1].size(); ++i)
				this->_requestHeader["Boundary"].append(1,lineSplited[1].at(i));
		}
		else if (lineSplited[0] == "Origin" || lineSplited[0] == "Host" || lineSplited[0] == "Referer")
		{
			lineSplited = lineSplited = split(stringTrim((*it), " \t\r\n\v\f"), ' ');
			lineSplited[0].erase(lineSplited[0].size() - 1);
			this->_requestHeader.insert(std::pair<std::string, std::string>(lineSplited[0], lineSplited[1]));
		}
		else if (!lineSplited.size())
		{
			if (this->_requestHeader.find("Transfer-Encoding") != this->_requestHeader.end() 
				&& this->_requestHeader.at("Transfer-Encoding") == "chunked")
				it = this->requestBodyChunkPaser(++it, requestLine);
			else
				it = this->requestBodyPaser(++it, requestLine);
			break ;
		}
		else
		{
			this->_requestHeader.insert(
				std::pair<std::string, std::string>(
					lineSplited[0], stringTrim(lineSplited[1], " \t\r\n\v\f")));
		}
	}
}

std::vector<std::string>::iterator	HttpRequest::requestBodyPaser(
	std::vector<std::string>::iterator it, std::vector<std::string> &requestLine)
{
	RequestBody	body;
	std::string	content;
	std::string fileName;
	std::string	fileType;
	std::string bodyContent;
	bool		isGetBody;

	this->_requestBody.clear();
	bodyContent = this->_raw.substr(this->_raw.find("\r\n\r\n") + 4);
	while (it != requestLine.end())
	{
		isGetBody = false;
		content.clear();
		fileName.clear();
		if (this->_requestHeader.find("Boundary") != this->_requestHeader.end()
			&& it->find(this->_requestHeader["Boundary"]) != std::string::npos)
			++it;
		if (it == requestLine.end()) {
			break;
		}
		if (*it == "\r")
			++it;
		while (this->_requestHeader["Content-Type"] == "multipart/form-data"
			&& it != requestLine.end()
			&& it->find(this->_requestHeader["Boundary"]) == std::string::npos)
		{
			if (it->find("Content-Disposition") != std::string::npos)
				fileName = it->substr(it->rfind('=') + 2, it->rfind('\"') - (it->rfind('=') + 2));
			else if (it->find("Content-Type") != std::string::npos)
				fileType = it->substr(it->find(':'), it->size() - it->find(":"));
			else if (it->find("\r") != std::string::npos && !isGetBody)
			{
				bodyContent = bodyContent.substr(bodyContent.find("\r\n\r\n") + 4);
				for (size_t index = 0; index < bodyContent.find("--" + this->_requestHeader.at("Boundary")); ++index)
					content.append(1, bodyContent.at(index));
				bodyContent = bodyContent.substr(bodyContent.find(this->_requestHeader.at("Boundary")));
				isGetBody = true;
			}
			++it;
		}
		while (this->_requestHeader["Content-Type"] != "multipart/form-data"
			&& it != requestLine.end())
				content += stringTrim(*it++, "\r") + '\n';
		if (this->_requestHeader["Content-Type"] == "multipart/form-data")
		{
			body.setMethod(UPLOAD);
			body.setFileName(fileName);
			body.setFileType(fileType);
		}
		else
			body.setMethod(PASSIN);
		body.setContent(content);
		if (this->_requestHeader.find("Content-Length") != this->_requestHeader.end())
			body.setContentLength(stringToint(this->_requestHeader.at("Content-Length")));
		if (!fileName.empty() || !content.empty())
			this->_requestBody.push_back(body);
	}
	return (it);
}

std::vector<std::string>::iterator	HttpRequest::requestBodyChunkPaser(std::vector<std::string>::iterator it, std::vector<std::string> &requestLine)
{
	bool 		isGetBody;
	bool 		isGetLength;
	RequestBody	body;
	std::string	fileName;
	std::string	content;

	isGetBody = false;
	isGetLength = false;
	this->_requestBody.clear();
	while (it != requestLine.end())
	{
		if (!isGetLength)
		{
			if (this->_requestHeader.at("Method") == "PUT")
				body.setMethod(UPLOAD);
			else
				body.setMethod(PASSIN);
			body.setFileType(this->_requestHeader.at("Transfer-Encoding"));
			fileName = this->_requestHeader.at("URL").substr(this->_requestHeader.at("URL").rfind('/') + 1);
			body.setFileName(fileName);
			body.setContentLength(hexadacimalToInt(stringTrim(*it, "\r\n").c_str()));
			isGetLength = true;
		}
		else
		{
			content = stringTrim(*it, "\r\n");
			body.setContent(content);
			isGetBody = true;
		}
		if (isGetLength && isGetBody)
		{
			this->_requestBody.push_back(body);
			isGetLength = false;
			isGetBody = false;
			memset((void *) &body, 0, sizeof(RequestBody));
			content.clear();
		}
		++it;
	}
	return (it);
}

void	HttpRequest::initHttpRequest(Config const &server)
{
	std::string pathURL;
	std::vector<std::map<std::string, std::string> >::const_iterator	it;

	pathURL = (this->_requestHeader.find("URL") != this->_requestHeader.end()) ? this->_requestHeader.at("URL") : "";
	if (pathURL.empty() || this->_requestHeader.find("Host") == this->_requestHeader.end())
		this->_badRequest = true;
	while (!this->_badRequest && this->_serverNum < (int) server.getServers().size())
	{
		std::vector<std::string> hosts = StringUtil::split(this->_requestHeader.at("Host"), ":");
		std::string hostName = hosts[0];
		std::string hostPort = "80";
		if (hosts.size() == 2) {
			hostPort = hosts[1];
		}
		const ServerConf *servConf = server.getServers().at(this->_serverNum);
		if (servConf->getServerName() == hostName && servConf->getListen() == hostPort)
			break ;
		++this->_serverNum;
	}
	if (this->_serverNum == (int) server.getServers().size())
		this->_badRequest = true;
	if (this->_badRequest)
		return ;
	this->_maxBody = server.getServers().at(this->_serverNum)->getClientSize();
	this->_methodAllow = server.getServers().at(this->_serverNum)->getMethod();
	this->_fileCGI.clear();
	if (this->_requestHeader.at("URL") == "/")
	{
		this->_path = server.getServers().at(this->_serverNum)->getRoot() + "/" + server.getIndex();
	}
	else
	{
		for (it = server.getServers().at(this->_serverNum)->getLocations().begin();
		it != server.getServers().at(this->_serverNum)->getLocations().end(); ++it)
		{
			if (it->find("path") != it->end() && pathURL.find(it->at("path")) != std::string::npos)
				this->initPath(it, pathURL, server);
			if (it->find("cgi_file") != it->end() && it->find("cgi_pass") != it->end())
			{
				if (this->innitCGIPath(it, pathURL))
					break ;
			}
		}
		if (this->_fileCGI.empty())
			this->_path = server.getServers().at(this->_serverNum)->getRoot() + pathURL;
	}
}

void	HttpRequest::initPath(
	std::vector<std::map<std::string, std::string> >::const_iterator it, std::string &pathURL, Config const &server)
{
	int	position;

	(void) server;
	if (it->find("return") != it->end()) {
		std::vector<std::string> ret = StringUtil::split(it->at("return"), " ");
		this->_return.push_back(ret[0]);
		this->_return.push_back(ret[1]);
	}
	if (it->find("root") != it->end())
	{
		position = pathURL.find(it->at("path"));
		pathURL.erase(position, it->at("path").length());
		pathURL.insert(position, it->at("root"));
	}
	if (it->find("client_max_size") != it->end())
		this->_maxBody = stringToint(it->at("client_max_size"));
	if (it->find("method_allow") != it->end())
	{
		std::vector<std::string> tmp = split(it->at("method_allow"), ',');
		for (std::vector<std::string>::iterator item = tmp.begin(); item != tmp.end(); ++item)
			this->_methodAllow.push_back(*item);
	}
}

bool	HttpRequest::innitCGIPath(std::vector<std::map<std::string,
	std::string> >::const_iterator it, std::string const &pathURL)
{
	std::string	fileCGI = it->at("cgi_file");
	std::string	newPath = it->at("cgi_pass");
	std::string	fileName = pathURL;

	if (pathURL.find('/') != std::string::npos)
		fileName = pathURL.substr(pathURL.rfind('/') + 1);
	if (fileCGI.find('*') == 0)
	{
		fileCGI.erase(0, 1);
		if (pathURL.rfind(fileCGI) == pathURL.length() - fileCGI.length() && pathURL.length() > fileCGI.length())
		{
			this->_path = it->at("cgi_pass");
			this->_fileCGI = it->at("cgi_file");
			if (it->find("client_max_size") != it->end())
				this->_maxBody = stringToint(it->at("client_max_size"));
			if (it->find("method_allow") != it->end())
			{
				std::vector<std::string> tmp = split(it->at("method_allow"), ',');
				for (std::vector<std::string>::iterator item = tmp.begin(); item != tmp.end(); ++item)
					this->_methodAllow.push_back(*item);
			}
			return (true);
		}
	}
	else if (fileCGI == fileName)
	{
		this->_path = newPath;
		this->_fileCGI = it->at("cgi_file");
		if (it->find("client_max_size") != it->end())
			this->_maxBody = stringToint(it->at("client_max_size"));
		return (true);
	}
	return (false);
}


static void	printMapKeyValue(std::map<std::string, std::string> const &map);

std::ostream	&operator<<(std::ostream &out, HttpRequest const &rhs)
{
	out << "==================== (HttpRequestHeader) ====================" << std::endl;
	printMapKeyValue(rhs.getRequestHeader());
	out << "=============================================================" << std::endl;
	out << "======================== (Attribute) ========================" << std::endl;
	out << "  • _path: " << rhs.getPath() << std::endl;
	out << "=============================================================" << std::endl;
	out << "===================== (HttpRequestBody) =====================" << std::endl;
	if (rhs.getRequestBody().size())
	{
		for (std::vector<RequestBody>::const_iterator it = rhs.getRequestBody().begin();
		it != rhs.getRequestBody().end(); ++it)
			out << *it;
	}
	else
		out << "            this request's body has no content" << std::endl;
	out << "=============================================================" << std::endl;
	out << "====================== (HttpRequestRaw) =====================" << std::endl;
	out << rhs.getRequestRaw() << std::endl;
	out << "=============================================================" << std::endl;
	return (out);
}

static void	printMapKeyValue(std::map<std::string, std::string> const &map)
{
	for (std::map<std::string, std::string>::const_iterator it = map.begin();
		it != map.end(); ++it)
		std::cout << "|" <<  it->first << "|: " << it->second << "|" << std::endl;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:20:21 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/25 05:03:04 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest(void) : _raw(""), _path("")
{
	std::cout << "(HttpRequest) Defualt constructor is called." << std::endl;
}
 
HttpRequest::HttpRequest(int socket, Config const &server)
{
	std::cout << "(HttpRequest) Constructor is called." << std::endl;
	this->readSocket(socket);
	std::cout << "paser" << std::endl;
	this->requestPaser();
	std::cout << "initpath" << std::endl;
	for (int index = 0; index < this->_requestHeader.at("Method").size(); ++index)
		this->_requestHeader.at("Method").at(index) = toupper(this->_requestHeader.at("Method").at(index));
	// std::cout << this->_requestHeader.at("Method") << std::endl;
	// TODO init file path with url + config file; FilePath
	this->initPath(server);
}

HttpRequest::HttpRequest(HttpRequest const &rhs)
{
	std::cout << "(HttpRequest) Copy constructor is called." << std::endl;
	*this = rhs;
}

HttpRequest::~HttpRequest(void)
{
	std::cout << "(HttpRequest) Destructor is called." << std::endl;
}

HttpRequest	&HttpRequest::operator=(HttpRequest const &rhs)
{
	std::cout << "(HttpRequest) Copy assignment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_raw = rhs._raw;
		this->_requestHeader = rhs._requestHeader;
		this->_requestBody = rhs._requestBody;
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
		throw (HttpRequest::CannotAllocateException());
	while (true)
	{
		readByte = read(socket, buffer, READSIZE);
		buffer[READSIZE] = '\0';
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

	requestLine = split(stringTrim(this->_raw, " \t\n\v\f\r"), '\n');
	for (std::vector<std::string>::iterator it = requestLine.begin();
	it != requestLine.end(); ++it)
	{
		lineSplited = split(stringTrim((*it), " \t\n\v\f\r"), ':');
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
			for (int i = lineSplited[1].find('=') + 1;
				lineSplited.size() > 1 && i < lineSplited[1].size(); ++i)
				this->_requestHeader["Boundary"].append(1,lineSplited[1].at(i));
		}
		else if (lineSplited[0] == "Origin" || lineSplited[0] == "Host" || lineSplited[0] == "Referer")
		{
			lineSplited = lineSplited = split(stringTrim((*it), " \t\n\v\f\r"), ' ');
			lineSplited[0].pop_back();
			this->_requestHeader.insert(std::pair<std::string, std::string>(lineSplited[0], lineSplited[1]));
		}
		else if (lineSplited[0] == "\r")
		{
			it = this->requestBodyPaser(++it, requestLine);
			break ;
		}
		else
		{
			this->_requestHeader.insert(
				std::pair<std::string, std::string>(
					lineSplited[0], stringTrim(lineSplited[1], " \t\n\v\f\r")));
		}
	}
}

// ------WebKitFormBoundaryfsGCxxQTQMWW7Sz0
// Content-Disposition: form-data; name="file"; filename="test123.sh"
// Content-Type: text/x-sh

// #!bin/bash

// ehco helo

// ------WebKitFormBoundaryfsGCxxQTQMWW7Sz0--

std::vector<std::string>::iterator	HttpRequest::requestBodyPaser(
	std::vector<std::string>::iterator it, std::vector<std::string> &requestLine)
{
	RequestBody	body;
	std::string	content;
	std::string fileName;
	std::string	fileType;
	std::string bodyContent;
	bool		isGetBody;

	bodyContent = this->_raw.substr(this->_raw.find("\r\n\r\n") + 4);
	while (it != requestLine.end())
	{
		isGetBody = false;
		content.clear();
		fileName.clear();
		if (this->_requestHeader.find("Boundary") != this->_requestHeader.end()
			&& it->find(this->_requestHeader["Boundary"]) != std::string::npos)
			++it;
		if (*it == "\r")
			++it;
		while (this->_requestHeader["Content-Type"] == "multipart/form-data"
			&& it != requestLine.end()
			&& it->find(this->_requestHeader["Boundary"]) == std::string::npos)
		{
			// std::cout << "-> " << *it << std::endl;
			if (it->find("Content-Disposition") != std::string::npos)
				fileName = it->substr(it->rfind('=') + 2, it->rfind('\"') - (it->rfind('=') + 2));
			else if (it->find("Content-Type") != std::string::npos)
				fileType = it->substr(it->find(':'), it->size() - it->find(":"));
			else if (it->find("\r") != std::string::npos && !isGetBody)
			{
				bodyContent = bodyContent.substr(bodyContent.find("\r\n\r\n") + 4);
				for (int index = 0; index < bodyContent.find("--" + this->_requestHeader.at("Boundary")); ++index)
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
		if (!fileName.empty() || !content.empty())
			this->_requestBody.push_back(body);
	}
	return (it);
}

void	HttpRequest::initPath(Config const &server)
{
	std::string path;
	int			count;

	count = 0;
	if (this->_requestHeader.at("URL") == "/")
	{
		while (count < server.getIndex().size())
		{
			this->_path.clear();
			this->_path = std::string(ROOT) + "/" + server.getIndex()[count++];
			if (!access(this->_path.c_str(), F_OK))
				break ;
		}
	}
	else
		this->_path = ROOT + this->_requestHeader.at("URL");
	// std::cout << "-path-> " << this->_path << std::endl;
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

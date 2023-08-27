#include "HttpRequest.hpp"

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
	// std::cout << this->_raw <<std::endl;
	delete [] buffer;
}

// ------WebKitFormBoundaryfsGCxxQTQMWW7Sz0
// Content-Disposition: form-data; name="file"; filename="test123.sh"
// Content-Type: text/x-sh

// #!bin/bash

// ehco helo

// ------WebKitFormBoundaryfsGCxxQTQMWW7Sz0--

void	HttpRequest::requestPaser(void)
{
	std::vector<std::string>	requestLine;
	std::vector<std::string>	lineSplited;
	std::string					content;

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
			this->_requestHeader.insert(std::pair<std::string, std::string>("Protool", lineSplited[2]));
		}
		else if (lineSplited[0] == "Content-Type" && lineSplited[1].find(';'))
		{
			lineSplited = split(stringTrim(lineSplited[1], " "), ';');
			this->_requestHeader.insert(std::pair<std::string, std::string>("Content-Type", lineSplited[0]));
			for (int i = lineSplited[1].find('=') + 1; i < lineSplited[1].size(); ++i)
				this->_requestHeader["Boundary"].append(1,lineSplited[1].at(i));
		}
		else if (lineSplited[1] == "Orgion" || lineSplited[1] == "Host" || lineSplited[1] == "Referer")
		{
			lineSplited = lineSplited = split(stringTrim((*it), " \t\n\v\f\r"), ' ');
			this->_requestHeader.insert(std::pair<std::string, std::string>(lineSplited[0], lineSplited[1]));
		}
		else if (lineSplited[0] == "\r")
		{
			content.clear();
			while (it != requestLine.end())
			{
				if (this->_requestHeader["Content-Type"] == "multipart/form-data"
					&& it->find(this->_requestHeader["Boundary"]) == std::string::npos)
					content += stringTrim(*it, "\r") + '\n';
				++it;
			}
			std::cout << content << std::endl;
			break ;
		}
		else
		{
			this->_requestHeader.insert(
				std::pair<std::string, std::string>(
					lineSplited[0], stringTrim(lineSplited[1], " \t\n\v\f\r")));
		}
	}
	for (std::map<std::string, std::string>::iterator it = this->_requestHeader.begin();
		it != this->_requestHeader.end(); ++it)
		std::cout << it->first << "-> " << it->second << std::endl;
}

	// if (this->_request.find("hello") != this->_request.end())
	// 	std::cout << "found1" << std::endl;
	// if (this->_request.find("Method") != this->_request.end())
	// 	std::cout << "found2" << std::endl;
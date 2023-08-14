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
	std::cout << stringTrim("abbabhellovaaavva", "avb") sts::endl;;
	exit(0);
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

	requestLine = split(stringTrim(this->_raw, " \t\n\v\f\r"), '\n');
	for (std::vector<std::string>::iterator it = requestLine.begin();
	it != requestLine.end(); ++it)
	{
		lineSplited = split(stringTrim((*it), " \t\n\v\f\r"), ':');
		if (it == requestLine.begin() && lineSplited.size() == 1)
		{
			lineSplited = split(lineSplited[0], ' ');
			this->_request.insert(std::pair<std::string, std::string>("Method", lineSplited[0]));
			this->_request.insert(std::pair<std::string, std::string>("URL", lineSplited[1]));
			this->_request.insert(std::pair<std::string, std::string>("Protool", lineSplited[2]));
		}
		else if (lineSplited[0] == "Content-Type" && lineSplited[1].find(';'))
		{
			lineSplited = split(stringTrim(lineSplited[1], " "), ';');
			this->_request.insert(std::pair<std::string, std::string>("Content-Type", lineSplited[0]));
			for (int i = lineSplited[1].find('=') + 1; i < lineSplited[1].size(); ++i)
				this->_request["Boundary"].append(1,lineSplited[1].at(i));
		}
		
		else if (lineSplited[1] == "Orgion" || lineSplited[1] == "Host" || lineSplited[1] == "Referer")
		{
			lineSplited = lineSplited = split(stringTrim((*it), " \t\n\v\f\r"), ' ');
			this->_request.insert(std::pair<std::string, std::string>(lineSplited[0], lineSplited[1]));
		}
		else if (lineSplited[0] == "\r")
		{
			while (it != requestLine.end())
			{
				if (this->_request["Content-Type"] == "multipart/form-data"
					&& it->find(this->_request["Boundary"]))
				++it;
			}
			break ;
		}
		else
		{
			this->_request.insert(
				std::pair<std::string, std::string>(
					lineSplited[0], stringTrim(lineSplited[1], " \t\n\v\f\r")));
		}
	}
	// if (this->_request.find("hello") != this->_request.end())
	// 	std::cout << "found1" << std::endl;
	// if (this->_request.find("Method") != this->_request.end())
	// 	std::cout << "found2" << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->_request.begin();
		it != this->_request.end(); ++it)
		std::cout << it->first << "-> " << it->second << std::endl;
}

// HttpRequest::HttpRequest(int socket)
// {
// 	std::vector<std::string>	request;
// 	std::vector<std::string>	line;
// 	std::vector<std::string>	subLine;
// 	char						buffer;
// 	int							readByte;

// 	std::cout << "(HttpRequest) Constructor is called." << std::endl;
// 	readByte = read(socket, buffer, READSIZE);
// 	if (readByte < 0)
// 		throw (HttpRequest::CannotReadSocketException());
// 	buffer[readByte] = '\0';
// 	std::cout << buffer << std::endl;

// 	// * print buffer;
// 	// std::cout << "print buffer" << std::endl;
// 	// std::cout << buffer << std::endl;
// 	{
// 		std::string stringBuff(buffer);
// 		request = split(stringTrim(stringBuff, " \t\n\v\f\r"), '\n');
// 	}
// 	// * print request;
// 	// std::cout << "print request" << std::endl;
// 	// for (std::vector<std::string>::iterator it = request.begin();
// 	// 	it != request.end(); ++it)
// 	// 	std::cout << *it <<std::endl;
// 	// std::cout << std::endl;
// 	for (std::vector<std::string>::iterator iterator = request.begin(); 
// 		 iterator != request.end(); ++iterator)
// 	{
// 		line = split(stringTrim(*iterator, " \t\n\v\f\r"), ' ');
// 		if (!line[0].compare("GET") || !line[0].compare("POST") || !line[0].compare("DELETE"))
// 		{
// 			this->_method = line[0];
// 			this->_url = line[1];
// 			this->_version = line[2];
// 		}
// 		else if (!line[0].compare("Host:"))
// 		{
// 			subLine = split(line[1], ':');
// 			if (subLine.size() == 2)
// 			{
// 				this->_host = subLine[0];
// 				this->_port = subLine[1];
// 			}
// 		}
// 		else if (!line[0].compare("Connection:"))
// 			this->_connection = line[1];
// 		// else if (!line[0].compare("sec-ch-ua:"))
// 		else if (!line[0].compare("sec-ch-ua-mobile:"))
// 		{
// 			if (line[1].find('1') != std::string::npos)
// 				this->_mobile = 1;
// 			else
// 				this->_mobile = 0;
// 		}
// 		else if (!line[0].compare("User-Agent:"))
// 		{
// 			for (int i = 1; i < line.size();)
// 			{
// 				this->_agent += line[i++];
// 				if (i < line.size())
// 					this->_agent += " ";
// 			}
// 		}
// 		else if (!line[0].compare("sec-ch-ua-platform:"))
// 		{
// 			for (size_t pos; line[1].find('\"') != std::string::npos;)
// 			{
// 				pos = line[1].find('\"');
// 				line[1].erase(pos, 1);
// 			}
// 			this->_platform = line[1];
// 		}
// 		else if (!line[0].compare("Accept:"))
// 		{
// 			this->_acceptStr = line[1];
// 			subLine = split(line[1], ',');
// 			for (std::vector<std::string>::iterator it = subLine.begin();
// 				it != subLine.end(); ++it)
// 				this->_accept.push_back(*it);
// 		}
// 		else if (!line[0].compare("Sec-Fetch-Site:"))
// 			this->_fetchSite = line[1];
// 		else if (!line[0].compare("Sec-Fetch-Mode:"))
// 			this->_fetchMode = line[1];
// 		else if (!line[0].compare("Sec-Fetch-Dest:"))
// 			this->_fetchDest = line[1];
// 		else if (!line[0].compare("Referer:"))
// 			this->_referer = line[1];
// 		else if (!line[0].compare("Accept-Encoding:"))
// 		{
// 			this->_encodeStr.clear();
// 			for (std::vector<std::string>::iterator it = line.begin() + 1;
// 				it != line.end(); ++it)
// 			{
// 				this->_encodeStr += *it;
// 				for (size_t pos; (*it).find(',') != std::string::npos;)
// 				{
// 					this->_encodeStr += " ";
// 					pos = (*it).find(',');
// 					(*it).erase(pos, 1);
// 				}
// 				this->_encode.push_back(*it);
// 			}
// 		}
// 		else if (!line[0].compare("Accept-Language:"))
// 		{
// 			this->_languageStr = line[1];
// 			subLine = split(line[1], ',');
// 			for (std::vector<std::string>::iterator it = subLine.begin();
// 				it != subLine.end(); ++it)
// 				this->_language.push_back(*it);
// 		}
// 		else if (!line[0].compare("Upgrade-Insecure-Requests:"))
// 			this->_insecure = line[1];
// 		else if (!line[0].compare("Range:"))
// 		{
// 			subLine = split(line[1], '=');
// 			this->_rangeStr = subLine[1];
// 			subLine = split(subLine[1], ',');
// 			for (std::vector<std::string>::iterator it = subLine.begin();
// 				it != subLine.end(); ++it)
// 				this->_range.push_back(stringTrim(*it, " "));
// 		}
// 		line.clear();
// 		subLine.clear();
// 	}
// }
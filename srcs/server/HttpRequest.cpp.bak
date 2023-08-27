/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:20:21 by sharnvon          #+#    #+#             */
/*   Updated: 2023/06/12 20:23:26 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

// static std::vector<std::string>	split(std::string const &string, char dilimeter);
// static std::string				stringTrim(std::string &string, std::string const &delimeters);
static void						printVector(std::vector<std::string> const &vector);

HttpRequest::HttpRequest(void) :
_method(""), _url(""), _version(""), _host(""), _port(""), _mobile(-1), _platform(""), _fetchSite(""), _fetchMode(""), _fetchDest(""), _referer(""), _connection(""), _agent(""), _insecure(""), _acceptStr(""), _encodeStr(""), _languageStr(""), _contentLength(""), _content("")
{
	std::cout << "(HttpRequest) Defualt constructor is called." << std::endl;
}
 
// ? GET /favicon.ico HTTP/1.1                                                      | <Medthod> <URL> <Version>;
// ? Host: localhost:8080                                                           | <Host>:<Port>;
// ? Connection: keep-alive                                                         | <Conection> | keep-alive, close;
//* sec-ch-ua: "Not.A/Brand";v="8", "Chromium";v="114", "Microsoft Edge";v="114"    | "<Brand>";v="<Significant-Version>";
//? sec-ch-ua-mobile: ?0                                                            | ?<Boolen> [0]Destop, [1]Mobile;
//* User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Geko) Chrome/114.0.0.0 Safari/537.36 Edg/114.0.1823.41;
//* Mozilla/5.0 (<system-information>) <platform> (<platform-details>) <extensions> | <Product> / <Product-Version> <Coment>;
//? sec-ch-ua-platform: "macOS"                                                     | <Platform>;
//? Accept: image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8 :)                | <MIME_Typy>/<MIME_Subtype>, <MIME_Type>;*, */*;q=<Quality-Value>;
//? Sec-Fetch-Site: same-origin                                                     | <Fetch> | cross-site, same-origin, same-site, none;
//? Sec-Fetch-Mode: no-cors                                                         | <Fetch-Mode> | core, navigate, no-cors, same-origin, websocket;
//? Sec-Fetch-Dest: image                                                           | <Fetch-Dest>;
//? Referer: http://localhost:8080/                                                 | <URL>;
//? Accept-Encoding: gzip, deflate, br                                              | <Fomat>;q=<Qvalues-Weighting(default = 1)>, | gzip, compress, deflate, br, identity, *;
//? Accept-Language: en-US,en;q=0.9                                                 | <Language>;q=<Q-Factor-Weighting>,...
// TODO POST<body>Medthod.

// TODO line:52 fix it read propery
HttpRequest::HttpRequest(int socket)
{
	std::vector<std::string>	request;
	std::vector<std::string>	line;
	std::vector<std::string>	subLine;
	char						buffer[100000];
	int							readByte;

	std::cout << "(HttpRequest) Constructor is called." << std::endl;
	
	// readByte = read(socket, buffer, READSIZE);
	// if (readByte < 0)
	// 	throw (HttpRequest::CannotReadSocketException());
	// buffer[readByte] = '\0';
	// std::cout << buffer << std::endl;
	this->readSocket(socket);
	this->requestPaser();
	// * print buffer;
	// std::cout << "print buffer" << std::endl;
	// std::cout << buffer << std::endl;
	{
		std::string stringBuff(this->_raw);
		request = split(stringTrim(stringBuff, " \t\n\v\f\r"), '\n');
	}
	// * print request;
	// std::cout << "print request" << std::endl;
	// for (std::vector<std::string>::iterator it = request.begin();
	// 	it != request.end(); ++it)
	// 	// std::cout << "|" << *it << "|" << std::endl;
	// std::cout << std::endl;
	for (std::vector<std::string>::iterator iterator = request.begin(); 
		 iterator != request.end(); ++iterator)
	{
		line = split(stringTrim(*iterator, " \t\r\n\v\f"), ' ');
		if (!line[0].compare("GET") || !line[0].compare("POST") || !line[0].compare("DELETE"))
		{
			this->_method = line[0];
			this->_url = line[1];
			this->_version = line[2];
		}
		else if (!line[0].compare("Host:"))
		{
			subLine = split(line[1], ':');
			if (subLine.size() == 2)
			{
				this->_host = subLine[0];
				this->_port = subLine[1];
			}
		}
		else if (!line[0].compare("Connection:"))
			this->_connection = line[1];
		// else if (!line[0].compare("sec-ch-ua:"))
		else if (!line[0].compare("sec-ch-ua-mobile:"))
		{
			if (line[1].find('1') != std::string::npos)
				this->_mobile = 1;
			else
				this->_mobile = 0;
		}
		else if (!line[0].compare("User-Agent:"))
		{
			for (int i = 1; i < line.size();)
			{
				this->_agent += line[i++];
				if (i < line.size())
					this->_agent += " ";
			}
		}
		else if (!line[0].compare("sec-ch-ua-platform:"))
		{
			for (size_t pos; line[1].find('\"') != std::string::npos;)
			{
				pos = line[1].find('\"');
				line[1].erase(pos, 1);
			}
			this->_platform = line[1];
		}
		else if (!line[0].compare("Accept:"))
		{
			this->_acceptStr = line[1];
			subLine = split(line[1], ',');
			for (std::vector<std::string>::iterator it = subLine.begin();
				it != subLine.end(); ++it)
				this->_accept.push_back(*it);
		}
		else if (!line[0].compare("Sec-Fetch-Site:"))
			this->_fetchSite = line[1];
		else if (!line[0].compare("Sec-Fetch-Mode:"))
			this->_fetchMode = line[1];
		else if (!line[0].compare("Sec-Fetch-Dest:"))
			this->_fetchDest = line[1];
		else if (!line[0].compare("Referer:"))
			this->_referer = line[1];
		else if (!line[0].compare("Accept-Encoding:"))
		{
			this->_encodeStr.clear();
			for (std::vector<std::string>::iterator it = line.begin() + 1;
				it != line.end(); ++it)
			{
				this->_encodeStr += *it;
				for (size_t pos; (*it).find(',') != std::string::npos;)
				{
					this->_encodeStr += " ";
					pos = (*it).find(',');
					(*it).erase(pos, 1);
				}
				this->_encode.push_back(*it);
			}
		}
		else if (!line[0].compare("Accept-Language:"))
		{
			this->_languageStr = line[1];
			subLine = split(line[1], ',');
			for (std::vector<std::string>::iterator it = subLine.begin();
				it != subLine.end(); ++it)
				this->_language.push_back(*it);
		}
		else if (!line[0].compare("Upgrade-Insecure-Requests:"))
			this->_insecure = line[1];
		else if (!line[0].compare("Range:"))
		{
			subLine = split(line[1], '=');
			this->_rangeStr = subLine[1];
			subLine = split(subLine[1], ',');
			for (std::vector<std::string>::iterator it = subLine.begin();
				it != subLine.end(); ++it)
				this->_range.push_back(stringTrim(*it, " "));
		}
		line.clear();
		subLine.clear();
	}
}

// HttpRequest::HttpRequest(int socket)
// {
// 	std::vector<std::string>	request;
// 	std::string					line;
// 	char						buffer[2];
// 	int							readBit;
//
// 	int count = 0;
// 	std::cout << "(HttpRequest) Constructor is called." << std::endl;
// 	while (1)
// 	{
// 		readBit = read(socket, buffer, 1);
// 		buffer[readBit] = '\0';
// 		if (!readBit)
// 			break ;
// 		if (buffer[0] == '\n')
// 		{
// 			request = split(line, ' ');
// 			std::cout << "-> " << line << std::endl;
// 			if (!request[0].compare("GET") || !request[0].compare("POST") || !request[0].compare("DELETE"))
// 			{
// 				this->_method = request[0];
// 				this->_url = request[1];
// 				this->_version = request[2];
// 			}
// 			else if (!request[0].compare("Host:"))
// 				this->_host = request[1];
// 			else if (!request[0].compare("Connection:"))
// 			{
// 				this->_connection = request[1];
// 			}
// 			// else if (request[0].compare("sec-ch-ua:"))
// 			// else if (request[0].compare("sec-ch-ua-mobile:"))
// 			// else if (request[0].compare("User-Agent:"))
// 			// else if (request[0].compare("sec-ch-ua-platform:"))
// 			// else if (request[0].compare("Accept:"))
// 			// else if (request[0].compare("Sec-Fetch-Site:"))
// 			// else if (request[0].compare("Sec-Fetch-Mode:"))
// 			// else if (request[0].compare("Sec-Fetch-Dest:"))
// 			// else if (request[0].compare("Referer:"))
// 			// else if (request[0].compare("Accept-Encoding:"))
// 			// else if (request[0].compare("Accept-Language:"))
// 			line.clear();
// 			std::cout << ++count << std::endl;
// 			continue ;
// 		}
// 		line += buffer;
// 	}
// }
//
// /*
// * [utility_function] funciton spliting word from string with delimeter and return by vector\<std::string\>.
// * • empty-string    -> empty_vector.
// * • empty-delimeter -> string.
// * • Successful      -> vector\<std::string\> that was seperated by delimeter.
// */
// static std::vector<std::string>	split(std::string const &string, char delimeter)
// {
// 	std::vector<std::string>	result;
// 	std::string					word;
// 	int							index;
//
// 	index = 0;
// 	if (string == "")
// 		return (result);
// 	if (delimeter == '\0')
// 	{
// 		result.push_back(string);
// 		return (result);
// 	}
// 	while (index < string.size() && string[index])
// 	{
// 		while (string[index] == delimeter)
// 			index++;
// 		if (index < string.size() && string[index])
// 		{
// 			word.clear();
// 			while (index < string.size() && string[index] && string[index] != delimeter)
// 				word += string[index++];
// 			result.push_back(word);
// 		}
// 	}
// 	return (result);
// }
//
// /*
// * [utility_function] funciton triming string from string with delimeter and return by std::string.
// * • empty-string     -> string.
// * • empty-delimetesr -> string.
// * • Successful       -> string that was trimed by delimeters.
// */
// static std::string	stringTrim(std::string &string, std::string const &delimeters)
// {
// 	std::string	result;
// 	int			start;
// 	int			end;
//
// 	start = 0;
// 	end = 0;
// 	result = "";
// 	if (string == "" || delimeters == "")
// 		return (string);
// 	for (int i = 0; i < string.length(); ++i)
// 	{
// 		if (delimeters.find(string[i]) == std::string::npos)
// 		{
// 			start = i;
// 			break ;
// 		}
// 	}
// 	for (int i = string.length() - 1 ; i >= 0; --i)
// 	{
// 		if (delimeters.find(string[i]) == std::string::npos)
// 		{
// 			end = i;
// 			break ;
// 		}
// 	}
// 	result = string.substr(start, ((end - start) + 1));
// 	return (result);
// }

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
	if (&rhs != this)
	{
		this->_method = rhs.getMetthod();
		this->_url = rhs.getUrl();
		this->_version = rhs.getVersion();
		this->_host = rhs.getHost();
		this->_port = rhs.getPort();
		this->_mobile = rhs.getMobile();
		this->_platform = rhs.getPlatForm();
		this->_fetchSite = rhs.getFetchSite();
		this->_fetchMode = rhs.getFetchMode();
		this->_fetchDest = rhs.getFetchDest();
		this->_referer = rhs.getReferer();
		this->_accept = rhs.getAccept();
		this->_encode = rhs.getEncode();
		this->_language = rhs.getLanguage();
		this->_connection = rhs.getConnection();
	}
	return (*this);
}

std::string const	&HttpRequest::getMetthod(void) const
{
	return (this->_method);
}

std::string const	&HttpRequest::getUrl(void) const
{
	return (this->_url);
}

std::string const	&HttpRequest::getVersion(void) const
{
	return (this->_version);
}

std::string const	&HttpRequest::getHost(void) const
{
	return (this->_host);
}

std::vector<std::string> const	&HttpRequest::getAccept (void) const
{
	return (this->_accept);
}

std::vector<std::string> const	&HttpRequest::getLanguage(void) const
{
	return (this->_language);
}

std::vector<std::string> const	&HttpRequest::getEncode(void) const
{
	return (this->_encode);
}

std::string const	&HttpRequest::getConnection(void) const
{
	return (this->_connection);
}

std::string const	&HttpRequest::getPort(void) const
{
	return (this->_port);
}

bool const	HttpRequest::getMobile(void) const
{
	return (this->_mobile);
}

std::string const	&HttpRequest::getPlatForm(void) const
{
	return (this->_platform);

}
std::string const	&HttpRequest::getFetchSite(void) const
{
	return (this->_fetchSite);
}

std::string const	&HttpRequest::getFetchMode(void) const
{
	return (this->_fetchMode);
}

std::string const	&HttpRequest::getFetchDest(void) const
{
	return (this->_fetchDest);
}

std::string const	&HttpRequest::getReferer(void) const
{
	return (this->_referer);
}

std::string const	&HttpRequest::getAgent(void) const
{
	return (this->_agent);
}

std::string const	&HttpRequest::getInsecure(void) const
{
	return (this->_insecure);
}

std::string const	&HttpRequest::getAcceptStr(void) const
{
	return (this->_acceptStr);
}

std::string const	&HttpRequest::getEncodeStr(void) const
{
	return (this->_encodeStr);
}

std::string const	&HttpRequest::getLanguageStr(void) const
{
	return (this->_languageStr);
}

std::string const	&HttpRequest::getRangeStr(void) const
{
	return (this->_rangeStr);
}

std::vector<std::string> const	&HttpRequest::getRange(void) const
{
	return (this->_range);
}


std::ostream	&operator<<(std::ostream &out, HttpRequest const &rhs)
{
	out << "Medthod  : |" << rhs.getMetthod() << "|" << std::endl;
	out << "URl      : |" << rhs.getUrl() << "|" << std::endl;
	out << "Version  : |" << rhs.getVersion() << "|" << std::endl;
	out << "Host     : |" << rhs.getHost() << "|" << std::endl;
	out << "Port     : |" << rhs.getPort() << "|" << std::endl;
	out << "Accept   : ";
	printVector(rhs.getAccept());
	out << "Language : ";
	printVector(rhs.getLanguage());
	out << "Conection: |" << rhs.getConnection() << "|" << std::endl;
	out << "Mobile   : |" << rhs.getMobile() << "|" << std::endl;
	out << "Platfrom : |" << rhs.getPlatForm() << "|" << std::endl;
	out << "Encode   : ";
	printVector(rhs.getEncode());
	out << "FetchSite: |" << rhs.getFetchSite() << "|" << std::endl;
	out << "FetchMode: |" << rhs.getFetchMode() << "|" << std::endl;
	out << "FetchDect: |" << rhs.getFetchDest() << "|" << std::endl;
	out << "Referer  : |" << rhs.getReferer() << "|" << std::endl;

	return (out);
}

static void	printVector(std::vector<std::string> const &vector)
{
	for (std::vector<std::string>::const_iterator it = vector.begin();
		it != vector.end(); ++it)
		std::cout <<  "|"  << *it << "| ";
	std::cout << std::endl;
}
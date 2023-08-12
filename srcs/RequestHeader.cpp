/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/08 23:20:21 by sharnvon          #+#    #+#             */
/*   Updated: 2023/06/12 20:23:26 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHeader.hpp"

// static std::vector<std::string>	split(std::string const &string, char dilimeter);
// static std::string				stringTrim(std::string &string, std::string const &delimeters);
static void						printVector(std::vector<std::string> const &vector);

RequestHeader::RequestHeader(void) :
_method(""), _url(""), _version(""), _host(""), _port(""), _mobile(-1), _platform(""), _fetchSite(""), _fetchMode(""), _fetchDest(""), _referer(""), _connection(""), _agent(""), _insecure(""), _acceptStr(""), _encodeStr(""), _languageStr(""), _contentLength(""), _content("")
{
	std::cout << "(RequestHeader) Defualt constructor is called." << std::endl;
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
RequestHeader::RequestHeader(int socket)
{
	std::vector<std::string>	request;
	std::vector<std::string>	line;
	std::vector<std::string>	subLine;
	char						buffer[10000];
	int							readByte;

	std::cout << "(RequestHeader) Constructor is called." << std::endl;
	readByte = read(socket, buffer, 9999);
	if (readByte < 0)
		throw (RequestHeader::CannotReadSocketException());
	buffer[readByte] = '\0';

	// struct sockaddr_in		addr;
	// socklen_t				len;

	// len = sizeof(addr);
	// memset(&addr, '\0', len);

	// getsockname(socket, (struct sockaddr *)&addr, &len);

	// std::cout << ntohs(addr.sin_port) << std::endl;
	// // std::cout << ntohl(addr.sin_addr.s_addr) << std::endl;
	// std::cout << inet_ntoa(addr.sin_addr) << std::endl;
	// exit(1);

	// * print buffer;
	// std::cout << "print buffer" << std::endl;
	// std::cout << buffer << std::endl;
	{
		std::string stringBuff(buffer);
		request = split(stringTrim(stringBuff, " \t\n\v\f\r"), '\n');
	}
	// * print request;
	// std::cout << "print request" << std::endl;
	// for (std::vector<std::string>::iterator it = request.begin();
	// 	it != request.end(); ++it)
	// 	std::cout << *it <<std::endl;
	// std::cout << std::endl;
	for (std::vector<std::string>::iterator iterator = request.begin(); 
		 iterator != request.end(); ++iterator)
	{
		line = split(stringTrim(*iterator, " \t\n\v\f\r"), ' ');
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

// RequestHeader::RequestHeader(int socket)
// {
// 	std::vector<std::string>	request;
// 	std::string					line;
// 	char						buffer[2];
// 	int							readBit;
//
// 	int count = 0;
// 	std::cout << "(RequestHeader) Constructor is called." << std::endl;
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

RequestHeader::RequestHeader(RequestHeader const &rhs)
{
	std::cout << "(RequestHeader) Copy constructor is called." << std::endl;
	*this = rhs;
}

RequestHeader::~RequestHeader(void)
{
	std::cout << "(RequestHeader) Destructor is called." << std::endl;
}

RequestHeader	&RequestHeader::operator=(RequestHeader const &rhs)
{
	std::cout << "(RequestHeader) Copy assignment operator is called." << std::endl;
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

std::string const	&RequestHeader::getMetthod(void) const
{
	return (this->_method);
}

std::string const	&RequestHeader::getUrl(void) const
{
	return (this->_url);
}

std::string const	&RequestHeader::getVersion(void) const
{
	return (this->_version);
}

std::string const	&RequestHeader::getHost(void) const
{
	return (this->_host);
}

std::vector<std::string> const	&RequestHeader::getAccept (void) const
{
	return (this->_accept);
}

std::vector<std::string> const	&RequestHeader::getLanguage(void) const
{
	return (this->_language);
}

std::vector<std::string> const	&RequestHeader::getEncode(void) const
{
	return (this->_encode);
}

std::string const	&RequestHeader::getConnection(void) const
{
	return (this->_connection);
}

std::string const	&RequestHeader::getPort(void) const
{
	return (this->_port);
}

bool const	RequestHeader::getMobile(void) const
{
	return (this->_mobile);
}

std::string const	&RequestHeader::getPlatForm(void) const
{
	return (this->_platform);

}
std::string const	&RequestHeader::getFetchSite(void) const
{
	return (this->_fetchSite);
}

std::string const	&RequestHeader::getFetchMode(void) const
{
	return (this->_fetchMode);
}

std::string const	&RequestHeader::getFetchDest(void) const
{
	return (this->_fetchDest);
}

std::string const	&RequestHeader::getReferer(void) const
{
	return (this->_referer);
}

std::string const	&RequestHeader::getAgent(void) const
{
	return (this->_agent);
}

std::string const	&RequestHeader::getInsecure(void) const
{
	return (this->_insecure);
}

std::string const	&RequestHeader::getAcceptStr(void) const
{
	return (this->_acceptStr);
}

std::string const	&RequestHeader::getEncodeStr(void) const
{
	return (this->_encodeStr);
}

std::string const	&RequestHeader::getLanguageStr(void) const
{
	return (this->_languageStr);
}

std::string const	&RequestHeader::getRangeStr(void) const
{
	return (this->_rangeStr);
}

std::vector<std::string> const	&RequestHeader::getRange(void) const
{
	return (this->_range);
}


std::ostream	&operator<<(std::ostream &out, RequestHeader const &rhs)
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
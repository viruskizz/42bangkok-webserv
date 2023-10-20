/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 10:46:29 by sharnvon          #+#    #+#             */
/*   Updated: 2023/10/20 12:58:54 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cookie.hpp"

Cookie::Cookie(void)
{
	std::cout << "(Cookie) Default constructor is called." << std::endl;
}

Cookie::~Cookie(void)
{
	std::cout << "(Cookie) Destructor is called." << std::endl;
}

Cookie::Cookie(Cookie const &src)
{
	std::cout << "(Cookie) Copy constructor is called." << std::endl;
}


Cookie	&Cookie::operator=(Cookie const &rhs)
{
	std::cout << "(Cookie) Copy assignment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_cookieList.clear();
		for (std::map<std::string, std::string>::const_iterator it = rhs.getCookieList().begin();
			it != rhs.getCookieList().end(); ++it)
			this->_cookieList.insert(std::pair<std::string, std::string>(it->first, it->second));
	}
	return (*this);
}


std::map<std::string, std::string> const	&Cookie::getCookieList(void) const
{
	return (this->_cookieList);
}

static bool	iskValidKeyLetter(std::string const &key);

std::string	Cookie::setCookieAndHeader(std::string const &cookieRequest)
{
	std::string					respondHeader;
	std::vector<std::string>	cookies;
	std::vector<std::string>	splited;

	respondHeader.clear();
	cookies = split(cookieRequest, ';');
	for (std::vector<std::string>::iterator it = cookies.begin();
		it != cookies.end(); ++it)
	{
		splited = split(stringTrim((*it), " \r\n"), '=');
		std::cout << "-> |" << splited.size() << "|:|" << splited.at(0) << "|" << splited.at(1) << std::endl;
		if (splited.size() == 2 && !isMapKeyFound(this->_cookieList, splited.at(0)) && iskValidKeyLetter(splited.at(0)))
		{
			this->_cookieList.insert(std::pair<std::string, std::string>(splited.at(0), splited.at(1)));
			respondHeader += "Set-Cookie: " + splited.at(0) + "=" + splited.at(1) + BREAK_LINE;
			std::cout << "[Debug][Cookie] set" << std::endl;
		}
		else if (splited.size() == 2 && isMapKeyFound(this->_cookieList, splited.at(0)) && splited.at(1) != this->_cookieList.at(splited.at(0)))
		{
			this->_cookieList.erase(splited.at(0));
			this->_cookieList.insert(std::pair<std::string, std::string>(splited.at(0), splited.at(1)));
			respondHeader += "Set-Cookie: " + splited.at(0) + "=" + splited.at(1) + BREAK_LINE;
		}
	}
	std::cout << "[Debug[Cookie]]" << std::endl;
	std::cout << respondHeader << std::endl;
	return (respondHeader);
}

static bool	iskValidKeyLetter(std::string const &key)
{
	std::string invalidKey = "()<>@,;:\\\"/[]?={}";
	int			index;

	while (index < key.size())
	{
		if (key.at(index) >= 0 && key.at(index) <= 32 || invalidKey.find(key.at(index)) != std::string::npos)
			return (false);
		index++;
	}
	return (true);
}

std::map<std::string, std::string>	cookieList;
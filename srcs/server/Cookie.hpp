#ifndef COOKIE_HPP
# define COOKIE_HPP

# include "../Webserv.hpp"
# include <iostream>
# include <map>
# include <string>

class Cookie
{
	public:
		Cookie(void);
		Cookie(Cookie const &src);
		~Cookie(void);
		Cookie	&operator=(Cookie const &rhs);

		std::map<std::string, std::string> const	&getCookieList(void) const;
		std::string									setCookieAndHeader(std::string const &cookieRequest);

	private:
		std::map<std::string, std::string>	_cookieList;
};

// std::ostream	&operator<<(std::ostream const &o, Cookie const &rhs);

#endif
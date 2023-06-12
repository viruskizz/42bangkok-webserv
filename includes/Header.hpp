/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 18:57:45 by sharnvon          #+#    #+#             */
/*   Updated: 2023/06/08 23:20:01 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <iostream>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>
# include <exception>

class Header
{
	public:
		Header(void);
		Header(int socket);
		Header(Header const &rhs);
		~Header(void);
		Header	&operator=(Header const &rhs);
		std::string const				&getMetthod(void) const;
		std::string const				&getUrl(void) const;
		std::string const				&getVersion(void) const;
		std::string const				&getHost(void) const;
		std::vector<std::string> const	&getAccept (void) const;
		std::vector<std::string> const	&getLanguage(void) const;
		std::string const				&getConnection(void) const;
		std::string const				&getPort(void) const;
		bool const						getMobile(void) const;
		std::string const				&getPlatForm(void) const;
		std::string const				&getFetchSite(void) const;
		std::string const				&getFetchMode(void) const;
		std::string const				&getFetchDest(void) const;
		std::string const				&getReferer(void) const;
		std::vector<std::string> const	&getEncode(void) const;


		class CannotReadSocketException : public std::exception
		{
			public:
				virtual const char * what() const throw () {
					return ("(Header) Error: cannot read from socket_intput.");
				}
		};

	private:
		std::string					_method;
		std::string					_url;
		std::string 				_version;
		std::string					_host;
		std::string					_port;
		bool						_mobile;
		std::string					_platform;
		std::string					_fetchSite;
		std::string					_fetchMode;
		std::string					_fetchDest;
		std::string					_referer;
		std::vector<std::string>	_accept;
		std::vector<std::string>	_encode;
		std::vector<std::string>	_language;
		std::string					_connection;
};

std::ostream	&operator<<(std::ostream &out, Header const &rhs);

#endif
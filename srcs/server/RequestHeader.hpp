/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHeader.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 18:57:45 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/12 13:04:37 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHEADER_HPP
# define REQUESTHEADER_HPP

# include <iostream>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>
# include <exception>
# include "../Webserv.hpp"

class RequestHeader
{
	public:
		RequestHeader(void);
		RequestHeader(int socket);
		RequestHeader(RequestHeader const &rhs);
		~RequestHeader(void);
		RequestHeader	&operator=(RequestHeader const &rhs);
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
		std::string const				&getAgent(void) const;
		std::string const				&getInsecure(void) const;
		std::string const				&getAcceptStr(void) const;
		std::string const				&getEncodeStr(void) const;
		std::string const				&getLanguageStr(void) const;
		// std::vector<std::string>	_range;
		// std::string					_rangeStr;
		// std::string					_contentLength;
		// std::string					_content;
		std::string const				&getRangeStr(void) const;
		std::vector<std::string> const	&getRange(void) const;


		class CannotReadSocketException : public std::exception
		{
			public:
				virtual const char * what() const throw () {
					return ("(RequestHeader) Error: cannot read from socket_intput.");
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
		std::string					_agent;
		std::string					_insecure;
		std::string					_acceptStr;
		std::string					_encodeStr;
		std::string					_languageStr;
		std::vector<std::string>	_range;
		std::string					_rangeStr;
		std::string					_contentLength;
		std::string					_content;
};

std::ostream	&operator<<(std::ostream &out, RequestHeader const &rhs);

#endif
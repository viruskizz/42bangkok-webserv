/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRespond.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:20:16 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/14 10:48:25 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERRESPOND_HPP
# define SERVERRESPOND_HPP

# include <iostream>
# include <vector>
# include "../Webserv.hpp"
# include "../conf/Config.hpp"
# include "HttpRequest.hpp"

class ServerRespond
{
	private:
		HttpRequest	_request;
		std::string		_contentType;
		std::string		_statusCode;
		std::string		_httpVersion;
		char			*_bodyContent;
		int				_contentLength;
		int				_headerLength;
		bool			_cgi;
		int				_code;
		std::string		_respondHeader;

	public:
		ServerRespond(void);
		ServerRespond(int fd, Config const &server);
		ServerRespond(ServerRespond const &src);
		~ServerRespond(void);
		ServerRespond		&operator=(ServerRespond const &rhs);
		std::string			getRespond(void) const;
		int					getRespondLength(void) const;
		HttpRequest const	&getRequest(void) const;
		std::string const	&getContentType(void) const;
		std::string const	&getStatusCode(void) const;
		std::string const	&getHttpVersion(void) const;
		char const			*getBodyContent(void) const;
		int					getBodyLength(void) const;
		int					getHeaderLength(void) const;
		void				setRespondHeader(std::string const &respondHeader);
		int					getCode(void) const;
		void				sendRepond(int socket) const;
};

std::ostream	&operator<<(std::ostream &out, ServerRespond const &rhs);

#endif
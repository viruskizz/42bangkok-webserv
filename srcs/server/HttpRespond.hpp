/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRespond.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 20:20:16 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/14 10:48:25 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPOND_HPP
# define HTTPRESPOND_HPP

# include <iostream>
# include <vector>
# include "../Webserv.hpp"
# include "../StatusCodeBody.hpp"
# include "../conf/Config.hpp"
# include "CommonGatewayInterface.hpp"
# include "HttpRequest.hpp"
# include "Cookie.hpp"

class HttpRespond
{
	private:
		std::string								_respondHeader;
		std::string								_bodyContent;
		int										_code;
		bool									_cgi;
		bool									_html;
		static std::map<int, std::string> const	_listStatusCode;
		static std::map<int, std::string> const	_statusCodeBody;
		static Cookie							_cookies;

		int			methodGET(HttpRequest const &request, Config const &server);
		int			methodPOST(HttpRequest const &request, Config const &server, int bodyIndex);
		int			methodPOSTUpload(HttpRequest const &request, Config const &server, int bodyIndex);
		int			methodDELETE(HttpRequest const &request);
		int			methodPUT(HttpRequest const &request);

		void		initHeader(HttpRequest const &request);
		void		setErrorPage(HttpRequest const &request, Config const &server);
		bool		listDirectory(HttpRequest const &request, Config const &server);
		int			readFile(std::string const &filename);

	public:
		HttpRespond(void);
		HttpRespond(HttpRequest &request, Config const &server, int bodyIndex);
		HttpRespond(HttpRespond const &src);
		~HttpRespond(void);
		HttpRespond			&operator=(HttpRespond const &rhs);
		std::string			getRespond(void) const;
		int					getRespondLength(void) const;
		std::string			getRespondHeader(void) const;
		std::string			getRespondContent(void) const;
		bool				getCGI(void) const;
		int					getCode(void) const;
		void				sendRepond(int socket) const;
};

std::ostream	&operator<<(std::ostream &out, HttpRespond const &rhs);

#endif
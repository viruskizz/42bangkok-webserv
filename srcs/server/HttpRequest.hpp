/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 18:57:45 by sharnvon          #+#    #+#             */
/*   Updated: 2023/10/21 08:32:20 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HttpRequest_HPP
# define HttpRequest_HPP

# include <iostream>
# include <vector>
# include <map>
# include <fcntl.h>
# include <unistd.h>
# include <cstring>
# include <exception>
# include "../Webserv.hpp"
# include "RequestBody.hpp"
# include "../conf/Config.hpp"

class RequestBody;
class HttpRequest
{
	public:
		HttpRequest(void);
		HttpRequest(int socket, Config const &server);
		HttpRequest(HttpRequest const &rhs);
		~HttpRequest(void);
		HttpRequest	&operator=(HttpRequest const &rhs);
		std::map<std::string, std::string> const	&getRequestHeader(void) const;
		std::vector<RequestBody> const				&getRequestBody(void) const;
		std::vector<std::string> const				&getMethodAllow(void) const;
		std::string	const							&getRequestRaw(void) const;
		std::string	const							&getPath(void) const;
		std::string const							&getFileCGI(void) const;
		std::vector<std::string> const				&getReturn(void) const;
		std::map<std::string, std::string> const	&getErrorPage(void) const;
		int											getServerNum(void) const;
		int											getMaxBody(void) const;
		void										setPath(std::string &path);

	private:
		std::string							_raw;
		std::string							_root;
		std::string							_path;
		std::string							_fileCGI;
		int									_serverNum;
		std::map<std::string, std::string>	_requestHeader;
		std::vector<RequestBody>			_requestBody;
		int									_maxBody;
		std::vector<std::string>			_methodAllow;
		bool								_badRequest;
		std::map<std::string, std::string>	_errorPage;
		std::vector<std::string>			_return;

		void								readSocket(int socket);
		void								requestPaser(void);
		void								initHttpRequest(Config const &server);
		void								initPath(
			std::vector<std::map<std::string, std::string> >::const_iterator it, std::string &pathURL, Config const &server);
		bool								innitCGIPath(
			std::vector<std::map<std::string, std::string> >::const_iterator it, std::string const &pathURL);
		std::vector<std::string>::iterator	requestBodyPaser(
			std::vector<std::string>::iterator it, std::vector<std::string> &requestLine);
		std::vector<std::string>::iterator	requestBodyChunkPaser(
			std::vector<std::string>::iterator it, std::vector<std::string> &requestLine);
};

std::ostream	&operator<<(std::ostream &out, HttpRequest const &rhs);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 18:57:45 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/25 05:03:03 by sharnvon         ###   ########.fr       */
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
		std::string	const							&getRequestRaw(void) const;
		std::string	const							&getPath(void) const;
		void										setPath(std::string &path);

		class CannotReadSocketException : public std::exception
		{
			public:
				virtual const char * what() const throw () {
					return ("(HttpRequest) Error: cannot read from socket_intput.");
				}
		};
		class CannotAllocateException : public std::exception
		{
			public:
				virtual const char * what() const throw () {
					return ("(HttpRequest) Error: cannot allocate the memory.");
				}
		};

	private:
		std::string							_path;
		std::string							_raw;
		std::map<std::string, std::string>	_requestHeader;
		std::vector<RequestBody>			_requestBody;
		// std::map<std::string, std::string>	_requestBody;

		void								readSocket(int socket);
		void								requestPaser(void);
		void								initPath(Config const &server);
		std::vector<std::string>::iterator	requestBodyPaser(
			std::vector<std::string>::iterator it, std::vector<std::string> &requestLine);
};

std::ostream	&operator<<(std::ostream &out, HttpRequest const &rhs);

#endif
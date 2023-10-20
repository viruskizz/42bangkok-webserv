/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 22:31:38 by sharnvon          #+#    #+#             */
/*   Updated: 2023/10/20 18:39:10 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

# include "../Webserv.hpp"

# include "../conf/Config.hpp"
# include "HttpRequest.hpp"
# include <fcntl.h>

extern char **environ;

class CommonGatewayInterface
{
	public:
		CommonGatewayInterface(void);
		CommonGatewayInterface(Config const &server, HttpRequest const &request);
		CommonGatewayInterface(CommonGatewayInterface const & src);
		~CommonGatewayInterface(void);
		CommonGatewayInterface &operator=(CommonGatewayInterface const &rhs);

		int			getStatusCode(void) const;
		std::string	executeCGI(Config const &server, HttpRequest const &request);
		void		printValue(void) const;


	private:
		std::string					_protocol;
		std::string					_scheme;
		std::string					_serverPort;
		std::string					_method;
		std::string					_pathInfo;
		std::string					_pathTranslated;
		std::string					_pathFileName;
		std::string					_scriptName;
		std::string					_fileName;
		std::string					_query;
		std::string					_clientName;
		std::string					_clientPort;
		std::string					_serverName;
		std::vector<std::string>	_environment;
		int							_cgiStatus;
		// Config						_server;
		// static std::map<std::string, std::string> const	_fileType;
		void			initScriptURI(Config const &server, HttpRequest const &request);
		void			buildEnvironment(Config const &server, HttpRequest const &request);
		char			**getExecutorPath(std::string const &exceLanguage);
		int				pathExecutor(char **execPath, std::vector<RequestBody> const &requestBody, int i);
		std::string		getExecutorLanguage(Config const &server, HttpRequest const &request);
		std::string		executeWithBody(HttpRequest const &request, char **executePath);
		std::string		errnoCheck(void);
};

std::ostream	&operator<<(std::ostream &out, CommonGatewayInterface const &rhs);

#endif
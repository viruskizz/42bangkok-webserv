/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBody.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 01:18:58 by sharnvon          #+#    #+#             */
/*   Updated: 2023/09/28 17:05:37 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTBODY_HPP
# define REQUESTBODY_HPP

# include <iostream>
# include "../Webserv.hpp"

class RequestBody
{
	public:
		RequestBody(void);
		RequestBody(RequestBody const &rhs);
		~RequestBody(void);
		RequestBody	&operator=(RequestBody const &rhs);

		int					getMethod(void) const;
		std::string const	&getFileName(void) const;
		std::string const	&getContent(void) const;
		int					getContentLength(void) const;
		void				setMethod(int input);
		void				setFileName(std::string &filename);
		void				setContent(std::string &content);
		void				setFileType(std::string &fileType);
		void				setContentLength(int length);
		
	private:
		int			_method;
		std::string	_fileName;
		std::string	_content;
		std::string	_fileType;
		int			_contentLength;
};

std::ostream	&operator<<(std::ostream &out, RequestBody const &rhs);

#endif
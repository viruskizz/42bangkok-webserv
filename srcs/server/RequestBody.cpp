/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestBody.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 01:30:55 by sharnvon          #+#    #+#             */
/*   Updated: 2023/08/22 21:00:20 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestBody.hpp"

RequestBody::RequestBody(void) : _method(0), _fileName(""), _content(""), _fileType("")
{
	std::cout << "(RequestBody) Defualt constructor is called." << std::endl;
}

RequestBody::RequestBody(RequestBody const &rhs)
{
	std::cout << "(RequestBody) Copy constructor is called." << std::endl;
	*this = rhs;
}

RequestBody::~RequestBody(void)
{
	std::cout << "(RequestBody) Destructor is called." << std::endl;
}

RequestBody	&RequestBody::operator=(RequestBody const &rhs)
{
	std::cout << "(RequestBody) Copy assignment operator is called." << std::endl;
	if (this != &rhs)
	{
		this->_method = rhs._method;
		this->_fileName = rhs._fileName;
		this->_content = rhs._content;
		this->_fileType = rhs._fileType;
	}
	return (*this);
}

int	RequestBody::getMethod(void) const
{
	return (this->_method);
}

std::string const	&RequestBody::getFileName(void) const
{
	return (this->_fileName);
}

std::string const	&RequestBody::getContent(void) const
{
	return (this->_content);
}

void	RequestBody::setMethod(int input)
{
	this->_method = input;
}

void	RequestBody::setFileName(std::string &filename)
{
	this->_fileName = filename;
}

void	RequestBody::setFileType(std::string &fileType)
{
	this->_fileType = fileType;
}

void	RequestBody::setContent(std::string &content)
{
	this->_content = content;
}

std::ostream	&operator<<(std::ostream &out, RequestBody const &rhs)
{
	out << "|Method: ";
	if (rhs.getMethod() == NONE)
		out << "none method is set." << std::endl;
	else if (rhs.getMethod() == PASSIN)
		out << "value passing." << std::endl;
	else if (rhs.getMethod() == UPLOAD)
		out << "file uploading." << std::endl;
	else
		out << "wrong method." << std::endl;
	out << "|fileName: |" << rhs.getFileName() << "|" << std::endl;;
	out << " |------------------------ content ------------------------|" << std::endl;
	out << rhs.getContent() << std::endl;
	out << " |-------------------------- end --------------------------|" << std::endl;
	return (out);
}

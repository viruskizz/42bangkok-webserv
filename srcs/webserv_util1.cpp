/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_util1.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 20:28:18 by sharnvon          #+#    #+#             */
/*   Updated: 2023/06/12 20:24:32 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <vector>

/*
* [utility_function] funciton triming string from string with delimeter and return by std::string.
* • empty-string     -> string.
* • empty-delimetesr -> string.
* • Successful       -> string that was trimed by delimeters.
*/
std::string	stringTrim(std::string &string, std::string const &delimeters)
{
	std::string	result;
	int			start;
	int			end;

	start = 0;
	end = 0;
	result = "";
	if (string == "" || delimeters == "")
		return (string);
	for (int i = 0; i < string.length(); ++i)
	{
		if (delimeters.find(string[i]) == std::string::npos)
		{
			start = i;
			break ;
		}
	}
	for (int i = string.length() - 1 ; i >= 0; --i)
	{
		if (delimeters.find(string[i]) == std::string::npos)
		{
			end = i;
			break ;
		}
	}
	result = string.substr(start, ((end - start) + 1));
	return (result);
}

/*
* [utility_function] funciton spliting word from string with delimeter and return by vector\<std::string\>.
* • empty-string    -> empty_vector.
* • empty-delimeter -> string.
* • Successful      -> vector\<std::string\> that was seperated by delimeter.
*/
std::vector<std::string>	split(std::string const &string, char delimeter)
{
	std::vector<std::string>	result;
	std::string					word;

	result.clear();
	if (string == "")
		return (result);
	if (delimeter == '\0')
	{
		result.push_back(string);
		return (result);
	}
	for (int i = 0; string[i];)
	{
		while (string[i] == delimeter)
			i++;
		if (string[i])
		{
			word.clear();
			while (string[i] && string[i] != delimeter)
				word += string[i++];
			result.push_back(word);
		}
	}
	return (result);
}
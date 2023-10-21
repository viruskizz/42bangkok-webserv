/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservUtil.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sharnvon <sharnvon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 20:28:18 by sharnvon          #+#    #+#             */
/*   Updated: 2023/10/20 21:46:56 by sharnvon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Webserv.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

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
	for (size_t i = 0; i < string.length(); ++i)
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
	if (end == start)
		return (result); 
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

char	*strjoin(char *str1, char *str2, int mode)
{
	char	*result;
	int		index;

	index = 0;
	if (!str1)
		result = new char[strlen(str2 + 1)];
	else
		result = new char[strlen(str1) + strlen(str2) + 1];
	for (int i = 0; str1 && str1[i]; ++i)
		result[index++] = str1[i];
	for (int i = 0; str2 && str2[i]; ++i)
		result[index++] = str2[i];
	result[index] = '\0';
	if (str1 && (mode == SJ_FFIR || mode == SJ_BOTH))
		delete[] str1;
	if (str2 && (mode == SJ_FSEC || mode == SJ_BOTH))
		delete[] str2;
	return (result);
}

char	*strjoin(char *str1, int lenStr1, char *str2, int lenStr2, int mode)
{
	char	*result;
	int		index;

	index = 0;
	result = new char[lenStr1 + lenStr2 + 1];
	for (int i = 0; str1 && i < lenStr1; ++i)
		result[index++] = str1[i];
	for (int i = 0; str2 && i < lenStr2; ++i)
		result[index++] = str2[i];
	result[index] = '\0';
	if (str1 && (mode == SJ_FFIR || mode == SJ_BOTH))
		delete[] str1;
	if (str2 && (mode == SJ_FSEC || mode == SJ_BOTH))
		delete[] str2;
	return (result);
}

std::string	intToString(int number)
{
	std::ostringstream	result;
	
	result << number;
	return (result.str());
}

int		stringToint(std::string string)
{
	std::stringstream	ss;
	int					result;

	ss << string;
	ss >> result;
	return (result);
}

char	*itoa(int number)
{
	std::string	num;
	char		*result;
	int			index;
	int			numberSize;

	num = intToString(number);
	numberSize = num.size();
	result = new char[numberSize + 1];
	index = 0;
	while (index < numberSize)
	{
		result[index] = num.at(index);
		index++;
	}
	result[index] = '\0';
	return (result);
}

int	stringsCount(char **strings)
{
	int count;

	count = 0;
	while (strings && strings[count])
		count++;
	return (count);
}

char	*stringDuplicate(char *string)
{
	char	*result;
	int		index;

	if (!string)
		return (NULL);
	index = 0;
	result = new char [strlen(string)];
	if (!result)
		return (NULL);
	while (string[index])
	{
		result[index] = string[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}

char	*stringDuplicate(std::string const &string)
{
	char	*result;
	size_t		index;

	index = 0;
	result = new char[string.size() + 1];
	if (!result)
		return (NULL);
	while (index < string.size())
	{
		result[index] = string[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}

void freeStrings2Star(char **strings)
{
	int	index;

	index = 0;
	while (strings && strings[index])
		delete [] strings[index++];
	delete [] strings;
}

char	**stringAdd(char **strings, char *string, int mode)
{
	char	**newStrings;
	int		length;
	int		index;

	length = stringsCount(strings);
	index = 0;
	if (string)
		newStrings = new char * [length + 2];
	else
		newStrings = new char * [length + 1];
	while (strings && strings[index])
	{
		newStrings[index] = stringDuplicate(strings[index]);
		index++;
	}
	if (string)
		newStrings[index++] = stringDuplicate(string);
	newStrings[index] = NULL;
	if (mode == SA_FSTS || mode == SA_BOTH)
		freeStrings2Star(strings);
	if (mode == SA_FSTR || mode == SA_BOTH)
		delete [] string;
	return (newStrings);
}

char	*stringTosChar(std::string const &string)
{
	char	*result;
	size_t	index;

	index = 0;
	if (string.empty())
		return (NULL);
	result = new char[string.size() + 1];
	while (index < string.size())
	{
		result[index] = string[index];
		index++;
	}
	result[index] = '\0';
	return (result);
}

char	**vectorStringToChar(std::vector<std::string> vector)
{
	char	**result;
	size_t	index;

	index = 0;
	result = new char * [vector.size() + 1];
	if (!result)
		return (NULL);
	while (index < vector.size())
	{
		result[index] = stringTosChar(vector.at(index));
		index++;
	}
	result[index] = NULL;
	return (result);
}

bool	findStringInVector(std::vector<std::string> const & vector, std::string const &string)
{
	for(std::vector<std::string>::const_iterator it = vector.begin(); it != vector.end(); ++it)
	{
		if (*it == string)
			return (true);
	}
	return (false);
}

int	hexadacimalToInt(const char *hexadecimal)
{
	int		result;
	int		index;
	char	letter;
	int		base;

	result = 0;
	index = strlen(hexadecimal) - 1;
	base = 1;
	if (!hexadecimal)
		return (-1);
	while (index >= 0)
	{
		if (isdigit(hexadecimal[index]))
			result += (hexadecimal[index] - '0') * base;
		else if (isalpha(hexadecimal[index]))
		{
			letter = toupper(hexadecimal[index]);
			if (letter >= 'A' && letter <= 'F')
				result += (letter - 55) * base;
		}
		else
			return (-1);
		base *= 16;
		index--;
	}
	return (result);
}

bool	isMapKeyFound(std::map<std::string, std::string> const &map, std::string const &key)
{
	if (map.find(key) != map.end())
		return (true);
	return (false);
}

bool	isStringFound(std::string const &string, std::string const &check)
{
	if (string.find(check) != std::string::npos)
		return (true);
	return (false);
}

bool	isStringInArray(char const **array, std::string const &string)
{
	int index;
	int len;

	index = 0;
	len = stringsCount((char **)array);
	while (index < len)
	{
		if (!strncmp(string.c_str(), array[index], string.length()))
			return (true);
		++index;
	}
	return (false);
}

void	exitWithError(char *errorMessage, int mode)
{
	if (mode == EE_PERR)
		perror(errorMessage);
	else if (mode == EE_NONE)
		std::cerr << errorMessage << std::endl;
	exit(EXIT_FAILURE);
}
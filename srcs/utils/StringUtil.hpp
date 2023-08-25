#pragma once
#include <iostream>
#include <string>
#include <vector>

class StringUtil
{
public:
	static std::vector<std::string> split(std::string const &string, char delimeter);
	static std::vector<std::string> split(std::string const &string, std::string const & delimeter);
	static std::string ltrim(std::string const &);
	static std::string rtrim(std::string const &);
	static std::string trim(std::string const &);
};

static int	findStrChar(std::string const & str, char del);
#pragma once
#include <iostream>
#include <string>
#include <vector>

class StringUtil
{
public:
	static std::vector<std::string> split(std::string const &string, char delimeter);
	static std::vector<std::string> split(std::string const &string, std::string const & delimeter);
};

static int	findStrChar(std::string const & str, char del);
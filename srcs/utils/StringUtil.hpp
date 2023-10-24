#pragma once
#include <iostream>
#include <string>
#include <vector>

class StringUtil
{
public:
	static std::vector<std::string> split(std::string const &, char);
	static std::vector<std::string> split(std::string const &, std::string const &);
	static std::string ltrim(std::string const &);
	static std::string rtrim(std::string const &);
	static std::string trim(std::string const &);
	static std::string join(std::vector<std::string> const, std::string const &);

	static std::string space(size_t);
};

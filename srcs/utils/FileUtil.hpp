

#pragma once
# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include "./StringUtil.hpp"

using std::string;
using std::vector;

class FileUtil
{
public:
	void lineByLine(std::ifstream & ifile, void (*func)(string const &, string const &));
};

#include "StringUtil.hpp"
static int	findStrChar(std::string const & str, char del);

std::vector<std::string> StringUtil::split(std::string const &string, char delimeter) {
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

std::vector<std::string> StringUtil::split(std::string const &str, std::string const & delimeter) {
	std::vector<std::string>	result;
	std::string					word;

	result.clear();
	if (str == "")
		return (result);
	for (int i = 0; str[i];)
	{
		while (findStrChar(delimeter, str[i]) > -1)
			i++;
		word.clear();
		while (str[i] && findStrChar(delimeter, str[i]) == -1)
			word += str[i++];
		if (!word.empty()) // * change
			result.push_back(word);
	}
	return (result);
}

// trim from start
std::string StringUtil::ltrim(std::string const & s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
		it++;
	std::string::const_reverse_iterator rit = s.rbegin();
    return std::string(it, rit.base());
}

// trim from end
std::string StringUtil::rtrim(std::string const & s) {
	std::string::const_iterator it = s.begin();
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
		rit++;
    return std::string(it, rit.base());
}

// trim from both ends
std::string StringUtil::trim(std::string const & s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
		it++;
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
		rit++;
    return std::string(it, rit.base());
}

std::string StringUtil::space(size_t size) {
	std::string sp = "";
	for (size_t i = 0; i < size; i++) {
		sp += " ";
	}
	return sp;
}

std::string StringUtil::join(std::vector<std::string> const strs, std::string const & del) {
	std::string result = "";
	for(size_t i = 0; i < strs.size(); i++) {
		result += strs.at(i);
		if (i != strs.size() - 1)
			result += del;
	}
	return result;
}


static int	findStrChar(std::string const & str, char find) {
	int i;

	i = 0;
	while (str[i]) {
		if (str[i] == find) {
			return i;
		}
		i++;
	}
	return -1;
}

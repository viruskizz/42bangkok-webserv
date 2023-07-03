#include "StringUtil.hpp"

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

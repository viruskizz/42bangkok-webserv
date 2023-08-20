#include "FileUtil.hpp"

void FileUtil::lineByLine(std::ifstream & ifile, void (*func)(string const &, string const &)) {
	string line;
	while (getline(ifile, line)) {
		if (!line.empty() && line.at(0) == '#') {
			continue;
		}
		vector<string> value = StringUtil::split(line, " \t");
		if (value.size() == 0) { continue;; }
		if (value.size() < 2) { throw "Invalid Key Value string"; }
		string key = value[0];
		string val = value[1];
		func(key, val);
	}
}

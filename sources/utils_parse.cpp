#include "../includes/include.hpp"

int fileToString(const std::string& filename, std::string& fileContents)
{
		std::ifstream input(filename.c_str());
		if (!input) {
			return (-1);
		}
		std::stringstream buffer;
		buffer << input.rdbuf();
		fileContents = buffer.str();
		return (1);
}
bool isComment(const std::string& line, const std::set<std::string>& commentPrefixes, int& multi)
{
	std::set<std::string>::const_iterator it;
	for (it = commentPrefixes.begin(); it != commentPrefixes.end(); ++it) {
		if (line.find(*it) == 0) {
			if (*it == "/*")
				multi = 1;
			else if (*it == "*/")
				multi = 0;
			return true;
		}
	}
	if (multi == 1)
		return true;
	return false;
}

std::string removeComments(const std::string& str)
{
		const char* comment_prefixes[] = {"//", "/*", "*/", "#", "<!--"};
		const int comment_prefixes_size = sizeof(comment_prefixes) / sizeof(comment_prefixes[0]);
		std::set<std::string>	commentPrefixes(comment_prefixes, comment_prefixes + comment_prefixes_size);
		std::istringstream		input(str);
		std::string				result;
		std::string				line;
		int						multi = 0;

		while (std::getline(input, line)) {
			if (!isComment(line, commentPrefixes, multi)) {
				if (result.length() > 0) {
					result += "\n";
				}
				result += line.substr(0, line.find_first_of(*(commentPrefixes.begin()), 0));
			}
		}
		return result;
}

bool isBracketBalanced(std::string fileContent)
{
	// Bracketlar açılıp kapanmış mı bakılacak
}
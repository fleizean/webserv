#include "../includes/include.hpp"

int fileToString(const string& filename, string& fileContents)
{
		ifstream input(filename.c_str());
		if (!input) {
			return (-1);
		}
		stringstream buffer;
		buffer << input.rdbuf();
		fileContents = buffer.str();
		return (1);
}

bool isComment(const string& line, const set<string>& commentPrefixes, int& multi)
{
	set<string>::const_iterator it;
	for (it = commentPrefixes.begin(); it != commentPrefixes.end(); ++it) {
		if (line.find(*it) == 0) {
			return true;
		}
	}
	if (multi == 1)
		return true;
	return false;
}

string removeComments(const string& str)
{
		const char* comment_prefixes[] = {"#"};
		const int comment_prefixes_size = sizeof(comment_prefixes) / sizeof(comment_prefixes[0]);
		set<string>	commentPrefixes(comment_prefixes, comment_prefixes + comment_prefixes_size);
		istringstream		input(str);
		string				result;
		string				line;
		int						multi = 0;

		while (getline(input, line)) {
			if (!isComment(line, commentPrefixes, multi)) {
				if (result.length() > 0) {
					result += "\n";
				}
				result += line.substr(0, line.find_first_of(*(commentPrefixes.begin()), 0));
			}
		}
		return result;
}

bool isBracketBalanced(string fileContent)
{
	// Bracketlar açılıp kapanmış mı bakılacak
    stack<char> s; // örn: push -> { if top -> { and current fileconten[i] == } so pop your { 
    for (size_t i = 0; i < fileContent.length(); ++i) {
    	char c = fileContent[i];
        if (c == '{') {
            s.push(c);
        }
        else if (c == '}') {
            if (s.empty() || s.top() != '{') {
                return false;
            }
            s.pop();
        }
    }
    return s.empty();
}
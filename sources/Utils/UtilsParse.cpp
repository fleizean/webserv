#include "../../includes/Include.hpp"
#include <sys/stat.h>

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

char	*get_cwd_buf()
{
	size_t	size;
	char	*buf;

	size = 2000;
	buf = NULL;
	buf = getcwd(buf, size);
	return (buf);
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

std::string	trim(std::string& line, const std::string& whitespace)
{
	size_t	line_begin = line.find_first_not_of(whitespace);
	if (line_begin == std::string::npos)
		return ("");
	size_t	line_range = line.find_last_not_of(whitespace) - line_begin + 1;

	return (line.substr(line_begin, line_range));
}

bool	isValidMethod(const std::string& word)
{
	if (word == "GET" || word == "POST" || word == "DELETE" || word == "OPTIONS")
		return true;
	return false;
}

static void	ft_skip_spacenl(const char *str, int *i)
{
	while ((str[*i] == ' ' || str[*i] == '\t' || str[*i] == '\n')
	|| (str[*i] == '\r' || str[*i] == '\v' || str[*i] == '\f'))
		(*i)++;
}

static int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	ft_skip_spacenl(str, &i);
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	return (num * sign);
}

unsigned int	strToIp(std::string strIp) {
	size_t  sep = 0;
	unsigned int   n;
	unsigned char  m[4];
	size_t  start = 0;
	if (strIp == "localhost")
		strIp = "127.0.0.1";
	for (unsigned int i = 3 ; i != std::numeric_limits<uint32_t>::max(); i--) {
		sep = strIp.find_first_of('.', sep);
		std::string str = strIp.substr(start, sep);
		n = ft_atoi(str.c_str());
		m[i] = static_cast<unsigned char>(n);
		sep++;
		start = sep;
	}
	unsigned final = *(reinterpret_cast<unsigned int *>(m));
	return final;
}

std::string removeAll(std::string str, const std::string &from)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from)) != std::string::npos)
	{
		str.erase(start_pos, from.length());
	}
	return str;
}
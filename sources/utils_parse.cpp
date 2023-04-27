#include "../includes/include.hpp"

static bool	ft_in_charset(char const c, const std::string &charset)
{
	int	i_charset;

	i_charset = 0;
	while (charset[i_charset])
	{
		if (c == charset[i_charset++])
			return true;
	}
	return false;
}

std::vector<std::string> ft_split(const std::string &str, const std::string &charset)
{
	std::vector<std::string> res;
	std::string			tmp;
	size_t			i;

	i = 0;
	while (i < str.length())
	{
		while (i < str.length() && ft_in_charset(str[i], charset))
			i++;
		if (i < str.length())
		{
			tmp = "";
			while (i < str.length() && !ft_in_charset(str[i], charset))
				tmp += str[i++];
			res.push_back(tmp);
		}
	}
	return res;
}


std::string get_content_file(const std::string &filename)
{
	std::ifstream myfile(filename.c_str());
	std::string content((istreambuf_iterator<char>(myfile)), istreambuf_iterator<char>());
	myfile.close();
	return content;
}


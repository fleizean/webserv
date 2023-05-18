#pragma once

#include <signal.h>
#include <iostream>
#include <iterator>
#include <set>
#include <dirent.h>
#include <fstream>
#include <list>
#include <sstream>
#include <fcntl.h>
#include <vector>
#include <stack>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include <dirent.h>
#include <filesystem>

using namespace std;
#include "Error.hpp"
#include "Color.hpp"

bool			isComment(const string& line, const set<string>& commentPrefixes, int& multi);
string			removeComments(const string& str);
int				fileToString(const string& filename, string& fileContents);
bool			isBracketBalanced(string fileContent);
void			parse_server(std::string _configContent);
std::string		trim(std::string& line, const std::string& whitespace);
bool            isValidMethod(const std::string& word);
unsigned int	strToIp(std::string strIp);
#pragma once

#include <signal.h>
#include <iostream>
#include <iterator>
#include <set>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <map>

using namespace std;

#include "Error.hpp"
#include "Color.hpp"
#include "WebServer.hpp"
#include "Server.hpp"
#include "Location.hpp"

bool			isComment(const string& line, const set<string>& commentPrefixes, int& multi);
string			removeComments(const string& str);
int				fileToString(const string& filename, string& fileContents);
bool			isBracketBalanced(string fileContent);
void			parse_server(std::string _configContent);
std::string		trim(std::string& line, const std::string& whitespace);
bool	isValidMethod(const std::string& word);
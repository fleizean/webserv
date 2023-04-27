#pragma once

#include <signal.h>
#include <iostream>
#include <iterator>
#include <set>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "color.hpp"
#include "error.hpp"
#include "WebServer.hpp"

Error err(0);

bool isComment(const std::string& line, const std::set<std::string>& commentPrefixes, int& multi);
std::string removeComments(const std::string& str);
int fileToString(const std::string& filename, std::string& fileContents);
bool isBracketBalanced(std::string fileContent);
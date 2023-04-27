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
#include "color.hpp"
#include "WebServer.hpp"

bool isComment(const string& line, const set<string>& commentPrefixes, int& multi);
string removeComments(const string& str);
int fileToString(const string& filename, string& fileContents);
bool isBracketBalanced(string fileContent);
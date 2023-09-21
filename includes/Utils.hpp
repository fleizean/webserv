#pragma once

#include "Include.hpp"
#include "ServerMembers.hpp"

bool			            isComment(const string& line, const set<string>& commentPrefixes, int& multi);
string			            removeComments(const string& str);
int				            fileToString(const string& filename, string& fileContents);
bool			            isBracketBalanced(string fileContent);
void			            parse_server(std::string _configContent);
std::string		            trim(std::string& line, const std::string& whitespace);
bool                        isValidMethod(const std::string& word);
unsigned int	            strToIp(std::string strIp);
std::string                 removeAll(std::string str, const std::string &from);
char	                    *get_cwd_buf();

std::string                 trimRight(const std::string& str, const std::string& spliter);
std::string                 trimLeft(const std::string& str, const std::string& spliter);
std::string                 trim(const std::string& str, const std::string& spliter);

#pragma once

#include "Include.hpp"
#include "ServerMembers.hpp"
#include <sys/stat.h>
#include <sys/time.h>

bool			            isComment(const string& line, const set<string>& commentPrefixes, int& multi);
string			            removeComments(const string& str);
int				            fileToString(const string& filename, string& fileContents);
bool			            isBracketBalanced(string fileContent);
void			            parse_server(std::string _configContent);
std::string		            trim(std::string& line, const std::string& whitespace);
bool                        isValidMethod(const std::string& word);
unsigned int	            strToIp(std::string strIp);
std::string                 removeSubstring(std::string str, const std::string &from);
char	                    *get_cwd_buf();

std::string                 trimRight(const std::string& str, const std::string& spliter);
std::string                 trimLeft(const std::string& str, const std::string& spliter);
std::string                 trim(const std::string& str, const std::string& spliter);
std::string                 getPwd();
std::string	                openNread(std::string file_path);
std::string                 setDate();
std::string	                add_headers_favicon(std::string _body);



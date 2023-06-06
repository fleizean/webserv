#pragma once

#include "Include.hpp"
#include "Request.hpp"
#include "ServerMembers.hpp"

class Cgi
{
private:
	char**							_envp;
	std::string						_requestHeader;
	std::string						_fileName;
	std::vector<std::string>		_env;
	std::string						_path;
	std::vector<std::string>		_postValues;
	ServerMembers*					_matchedServer;
	Request							_request;
public:
	Cgi();
	Cgi(char** envp, std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues, ServerMembers* matchedServer);
	~Cgi();

	std::string cgiExecute();
	void initOthersEnvironment(char* cwd);
	void extractKeyValues();
};
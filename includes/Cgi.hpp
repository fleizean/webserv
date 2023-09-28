#pragma once

#include "Include.hpp"
#include "Request.hpp"
#include "ServerMembers.hpp"

class Cgi
{
private:
	std::string						_requestHeader;
	std::string						_fileExecutePath;
	std::string						_keyValue;
	std::string						_cgiPath;
	std::vector<std::string>		_env;
	std::string						_cgiExecutePath;
	std::string						_multiBody;
	std::vector<std::string>		_postValues;
	ServerMembers*					_matchedServer;
	Request							_request;
public:
	Cgi();
	Cgi(std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues, ServerMembers* matchedServer, std::string cgiPath, std::string multiBody);
	~Cgi();

	std::string cgiExecute();
	void initOthersEnvironment();
	void extractKeyValues();
};
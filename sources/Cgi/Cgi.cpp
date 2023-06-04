#include "../../includes/Cgi.hpp"

Cgi::Cgi()
{
    _envp = NULL;
    _requestHeader = "";
    _request = NULL;
    _path = "";
    _postValues = std::vector<std::string>();
    _fileName = "";
}

Cgi::Cgi(char** envp, std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues) 
{
    _envp = envp;
    _requestHeader = m_request;
    _request = req;
    _path = path;
    _postValues = postValues;
    _fileName = fileName;
}

Cgi::~Cgi() 
{
    if (this->_envp)
	{
		for (int i = 0; this->_envp[i]; i++)
			free(this->_envp[i]);
		free(this->_envp);
	}
	this->_env.clear();
}

void Cgi::extractKeyValues() {
    for(std::vector<std::string>::iterator it = _postValues.begin(); it != _postValues.end(); it++)
        _env.push_back(*it);
}

void Cgi::initOthersEnvironment()
{
    _env.push_back("CONTENT_TYPE text/html");
    _env.push_back("DOCUMENT_ROOT");
    _env.push_back("CONTENT_LENGTH");
    _env.push_back("HTTP_COOKIE = none");
    _env.push_back("HTTP_HOST = off");
    _env.push_back("HTTP_REFERER");
    _env.push_back("HTTP_USER_AGENT");
    _env.push_back("HTTPS = off");
    _env.push_back("PATH" + std::string(_cwd));
    _env.push_back("QUERY_STRING");
    _env.push_back("REMOTE_ADDR");
    _env.push_back("REMOTE_HOST");
    _env.push_back("REMOTE_PORT");
    _env.push_back("REMOTE_USER");
    _env.push_back("REQUEST_METHOD");
    _env.push_back("REQUEST_URI" + std::string(_cwd));
    _env.push_back("SCRIPT_FILENAME" ); 
    _env.push_back("SCRIPT_NAME");
    _env.push_back("SERVER_ADMIN");
    _env.push_back("SERVER_NAME");
    _env.push_back("SERVER_PORT");
    _env.push_back("SERVER_SOFTWARE");
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("REDIRECT_STATUS=200");
}


std::string Cgi::cgiExecute()
{
    Error err(0);

    extractKeyValues();
    if (getcwd(_cwd, sizeof(_cwd)) == NULL)
        err.setAndPrint(51, "Cgi::cgiExecute");
    
    exit(1);

    return NULL;
}

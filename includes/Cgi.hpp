#pragma once

#include "Include.hpp"
#include "Request.hpp"

class Cgi
{
private:
    char* _cwd;
    char** _envp;
    std::string _requestHeader;
    std::string _fileName;
    Request _request;
    std::vector<std::string> _env;
    std::string _path;
    std::vector<std::string> _postValues;
    
public:
    Cgi();
    Cgi(char** envp, std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues);
    ~Cgi();

    std::string cgiExecute();
    void initOthersEnvironment();
    void extractKeyValues();
};
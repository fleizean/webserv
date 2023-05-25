#pragma once

#include "Include.hpp"

class Server;

class Cgi{
    public:
        Cgi();
        ~Cgi();

        std::string cgiExecute(std::string location, std::string executable ,std::string	postname[], std::string	postvalue[], int j,std::string m_request, int code);
};

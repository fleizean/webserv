#pragma once

#include "include.hpp"

class WebServer
{
private:
    std::string _conf_path;
public:
    /* default constructor */
    WebServer();
    /* copy constructor */
    WebServer(WebServer const &rhs);
    /* destructor */
    ~WebServer();
    /* operator */
    WebServer &operator=(WebServer const &rhs);

    /* methods */
    void FileChecker(const std::string &conf_path);
    
   // void BracketChecker();
};
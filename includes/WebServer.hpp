#pragma once

#include "include.hpp"

class WebServer
{
private:
    string _configContent;
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
    void FileChecker(const string &conf_path);
    
   // void BracketChecker();
};
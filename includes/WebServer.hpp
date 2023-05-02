#pragma once

#include "include.hpp"

class WebServer
{
private:
    string _configContent;

    bool serverBlock;
    bool mainBlock;
    bool locationBlock;
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
    void split_server(std::string _configContent);
    void parse_server();

    void endScopeConf();


    void parseMainArea(std::string& line);
    void parseServerArea(std::string& line);
    void parseLocationArea(std::string& line);
};
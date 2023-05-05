#pragma once

#include "include.hpp"

class WebServer
{
private:
    string _configContent;

    /* for control */
    bool serverBlock;
    bool mainBlock;
    bool locationBlock;

    std::map<long, Server> _servers;
    std::map<long, Server *>	_sockets;
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

    /* First Area */
    void endScopeConf();

    /* Main Blocks for Parse */
    void parseMainArea(std::string& line);
    void parseServerArea(std::string& line);
    void parseLocationArea(std::string& line);

    /* Server Parse */
    void parseLocation(std::stringstream& ss);
    void parseListen(std::stringstream& ss);
    void parseServerName(std::stringstream& ss);
    void parseRoot(std::stringstream& ss);
    void parseIndex(std::stringstream& ss);
    void parseCgi(std::stringstream& ss);
    void parseErrorPage(std::stringstream& ss);
    void parseMaxClientBodySize(std::stringstream& ss);

    /* Location Parse */
    void parseAllowedMethods(std::stringstream& ss);
    void parseReturn(std::stringstream& ss);
};
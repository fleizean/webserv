#pragma once

#include "include.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "ConfigMembers.hpp"

class WebServer
{
private:
    string _configContent;
    string _tmpLocationUri;

    /* for control */
    bool serverBlock;
    bool mainBlock;
    bool locationBlock;

    std::vector<Server*> _parsedServers;

    Location newLocation;
    /*std::map<long, Server> _servers;
    std::map<long, Server *>	_sockets;*/

public:
    /* default constructor */
    WebServer();
    /* copy constructor */
    WebServer(WebServer const &rhs);
    /* destructor */
    ~WebServer();
    /* operator */
    WebServer &operator=(WebServer const &rhs);
    std::vector<Server*> getConfig(void);

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

    /* ConfigMembers Parse */
    void parseRoot(std::stringstream& ss, ConfigMembers &cm);
    void parseMaxClientBodySize(std::stringstream& ss, ConfigMembers& cm);
    void parseErrorPage(std::stringstream& ss, ConfigMembers& cm);
    void parseIndex(std::stringstream& ss, ConfigMembers& cm);
    void parseAutoIndex(std::stringstream& ss, ConfigMembers& cm);
    void parseCgi(std::stringstream& ss, ConfigMembers &cm);

    /* Server Parse */
    void parseListen(std::stringstream& ss, Server &srvr);
    void parseServerName(std::stringstream& ss, Server &srvr);
    void parseLocation(std::stringstream& ss, Server &srvr);
    /* Location Parse */
    void parseAllowedMethods(std::stringstream& ss, Location &lctn);
    void parseReturn(std::stringstream& ss, Location &lctn);


    /* Print All */
    void printAll();
};
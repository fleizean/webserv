#pragma once

#include "include.hpp"

class Server;
class Location;


class WebServer
{
private:
    string _configContent;

    /* for control */
    bool serverBlock;
    bool mainBlock;
    bool locationBlock;

    std::vector<Server> _parsedServers;

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

    /* Server and Location Parsing */
    void parseListen(std::stringstream& ss, Server &srvr);
    void parseRoot(std::stringstream& ss, ConfigMembers &cm);
    void parseServerName(std::stringstream& ss, Server &srvr);
    void parseMaxClientBodySize(std::stringstream& ss, ConfigMembers& cm);
    void parseErrorPage(std::stringstream& ss, ConfigMembers& cm);
    void parseLocation(std::stringstream& ss, Server &srvr);
    void parseIndex(std::stringstream& ss, ConfigMembers& cm);
    void parseAutoIndex(std::stringstream& ss, ConfigMembers& cm);
    void parseCgi(std::stringstream& ss, ConfigMembers &cm);

    /* Location Parse */
    void parseAllowedMethods(std::stringstream& ss, Location &lctn);
    void parseRoot(std::stringstream& ss, Location &lctn);
    void parseErrorPage(std::stringstream& ss, Location &lctn);
    void parseMaxClientBodySize(std::stringstream& ss, Location &lctn);
    void parseIndex(std::stringstream& ss, Location &lctn);
    void parseReturn(std::stringstream& ss, Location &lctn);
};
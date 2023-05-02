#pragma once

#include "include.hpp"

class Server
{
private:
    string _listen;
    string _serverName;
    string _accessLog;
    string _root;

public:
    Server();
    Server(Server const &rhs);
    Server &operator=(Server const &rhs);

    ~Server();

    string const &getListen() const;
    string const &getServername() const;
    string const &getAccesslog() const;
    string const &getRoot() const;

    void setListen(std::string const &listen);
    void setServerName(std::string const &serverName);
    void setAccessLog(std::string const &accessLog);
    void setRoot(std::string const &root);
    
};
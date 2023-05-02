#include "Server.hpp"

Server::Server() : _listen(), _serverName(), _accessLog(), _root() { }

Server::Server(Server const &rhs) : _listen(rhs._listen), _serverName(rhs._serverName), _accessLog(rhs._accessLog), _root(rhs._root) { }

Server &Server::operator=(Server const &rhs)
{
    this->_listen = rhs._listen;
    this->_serverName = rhs._serverName;
    this->_accessLog = rhs._accessLog;
    this->_root = rhs._root;
}

/* <---------------> Set Area <---------------> */

void Server::setListen(std::string const &listen)
{
    this->_listen = listen;
}

void Server::setServerName(std::string const &serverName)
{
    this->_serverName = serverName;
}

void Server::setAccessLog(std::string const &accessLog)
{
    this->_accesLog = accesLog;
}

void Server::setRoot(std::string const &root)
{
    this->_root = root;
}
/* <------------------------------------------> */

/* <---------------> Get Area <---------------> */

string const &Server::getListen() const
{
    return this->_listen;
}

string const &Server::getServerName() const
{
    return this->_serverName;
}

string const &Server::getAcceslog() const
{
    return this->_accesLog;
}

string const &Server::getRoot() const
{
    return this->_root;
}

/* <------------------------------------------> */
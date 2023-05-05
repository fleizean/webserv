#include "Server.hpp"

Server::Server() : _listen(), _cgiPath(), _root(), _index(), _accessLog(), _errorPage(), _maxClientBodySize(), _location(), _locationMap() { }

Server::Server(Server const &rhs) : _listen(rhs._listen), _cgiPath(rhs._cgiPath), _root(rhs._root), _index(rhs._index), _accessLog(_accessLog), _errorPage(), _maxClientBodySize(_maxClientBodySize), _location(_location), _locationMap(_locationMap) { }

Server &Server::operator=(Server const &rhs)
{
    this->_listen = rhs._listen;
    this->_cgiPath = rhs._cgiPath;
    this->_root = rhs._root;
    this->_index = rhs._index;
    this->_accessLog = rhs._accessLog;
    this->_errorPage = rhs._errorPage;
    this->_maxClientBodySize = rhs._maxClientBodySize;
    this->_location = rhs._location;
    this->_locationMap = rhs._locationMap;
}

/* <---------------> Get Area <---------------> */

std::vector<t_listen> const &Server::getListen() const
{
    return this->_listen;
}

std::string const &Server::getCgiPath() const
{
    return this->_cgiPath;
}

std::string const &Server::getRoot() const
{
    return this->_root;
}

std::string const &Server::getIndex() const
{
    return this->_index;
}

std::string const &Server::getAccessLog() const
{
    return this->_accessLog;
}

std::string const &Server::getErrorPage() const
{
    return this->_errorPage;
}

std::string const &Server::getMaxClientBodySize() const
{
    return this->_maxClientBodySize;
}

std::string const &Server::getLocation() const
{
    return this->_location;
}

std::map<long, Location> const &Server::getLocationMap() const
{
    return this->_locationMap;
}

/* <---------------> Set Area <---------------> */

void Server::setCgiPath(std::string const &cgiPath)
{
    this->_cgiPath = cgiPath;
}

void Server::setRoot(std::string const &root)
{
    this->_root = root;
}

void Server::setIndex(std::string const &index)
{
    this-> _index = index;
}

void Server::setAccessLog(std::string const &accessLog)
{
    this->_accessLog = accessLog;
}

void Server::setErrorPage(std::string const &errorPage)
{
    this->_errorPage = errorPage;
}

void Server::setMaxClientBodySize(std::string const &maxClientBodySize)
{
    this->_maxClientBodySize = maxClientBodySize;
}

void Server::setLocation(std::string const &location)
{
    this->_location = location;
}
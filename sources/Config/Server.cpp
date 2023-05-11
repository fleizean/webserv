#include "../../includes/Server.hpp"

Server::Server() : _host(0), _port(80) {}

Server::Server(Server const &rhs) : _host(rhs._host), _port(rhs._port) {}

Server &Server::operator=(Server const &rhs)
{
    this->_host = rhs._host;
    this->_port = rhs._port;

    return (*this);
}

Server::~Server() { }

/* <---------------> Get Area <---------------> */

unsigned int const &Server::getHost() const { return this->_host; }
std::string const &Server::getLocationUri() const { return this->_locationUri; }
int const &Server::getPort() const { return this->_port; }
std::vector<Location *>& Server::getLocations() { return _locations; }
const std::vector<std::string>& Server::getServerName() { return _serverName; }
std::map<std::string, std::string>& Server::getCgiPaths() { return _cgiPaths; }
ConfigMembers& Server::getConfigMembers() { return this->_members; }

/* <------------------------------------------> */

/* <---------------> Set Area <---------------> */

void Server::setHost(unsigned int const &host) { this->_host = host; }

void Server::setPort(int const &port) { this->_port = port; }

void Server::setLocationUri(std::string const &uri) { this->_locationUri = uri; }

/* <------------------------------------------> */

void Server::setServerName(const std::string& name){
    this->_serverName.push_back(name);
}

#include "../includes/Server.hpp"

Server::Server() : _host("0.0.0.0"), _port(80) {}

Server::Server(Server const &rhs) : _host(rhs._host), _port(rhs._port) {}

Server &Server::operator=(Server const &rhs)
{
    this->_host = rhs._host;
    this->_port = rhs._port;

    return (*this);
}

Server::~Server() { }

/* <---------------> Get Area <---------------> */

std::string const &Server::getHost() const { return this->_host; }
int const &Server::getPort() const { return this->_port; }
std::vector<Location>& Server::getLocations() { return _locations; }
const std::vector<std::string>& Server::getServerName() { return _serverName; }
std::map<std::string, std::string>& Server::getCgiPaths() { return _cgiPaths; }
ConfigMembers& Server::getConfigMembers() { return this->_members; }

/* <------------------------------------------> */

/* <---------------> Set Area <---------------> */

void Server::setHost(std::string const &host) { this->_host = host; }
void Server::setPort(int const &port) { this->_port = port; }

/* <------------------------------------------> */

void Server::setServerName(const std::string& name){
    this->_serverName.push_back(name);
}
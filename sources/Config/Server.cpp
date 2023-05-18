#include "../../includes/Server.hpp"

Server::Server()
{
    _listen.host = 0;
    _listen.port = 80;
}

Server::Server(Server const &rhs) : _listen(rhs._listen) {}

Server &Server::operator=(Server const &rhs)
{
    this->_listen = rhs._listen;

    return (*this);
}

Server::~Server() { }

/* <---------------> Get Area <---------------> */

t_listen    Server::getListen() const { return this->_listen; }
std::string const &Server::getLocationUri() const { return this->_locationUri; }
std::vector<Location *>& Server::getLocations() { return _locations; }
const std::vector<std::string>& Server::getServerName() { return _serverName; }
std::string const &Server::getCgiPath() const { return _cgiPath; }
ConfigMembers& Server::getConfigMembers() { return this->_members; }
std::string const &Server::getHost() { return this->_host; }

/* <------------------------------------------> */

/* <---------------> Set Area <---------------> */


void Server::setLocationUri(std::string const &uri) { this->_locationUri = uri; }

void Server::setHost(std::string const &host)
{
    _host = host;
}

void Server::setHost(unsigned int const &host)
{
    _listen.host = host;
}

void Server::setPort(int const &port)
{
    _listen.port = port;
}

void Server::setCgiPath(const std::string& path) { _cgiPath = path; }


/* <------------------------------------------> */

void Server::setServerName(const std::string& name){
    this->_serverName.push_back(name);
}

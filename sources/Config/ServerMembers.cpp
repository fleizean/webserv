#include "../../includes/ServerMembers.hpp"

ServerMembers::ServerMembers()
{
    _listen.host = 0;
    _listen.port = 80;
}

ServerMembers::ServerMembers(ServerMembers const &rhs) : _listen(rhs._listen) {}

ServerMembers &ServerMembers::operator=(ServerMembers const &rhs)
{
    this->_listen = rhs._listen;

    return (*this);
}

ServerMembers::~ServerMembers() { }

/* <---------------> Get Area <---------------> */

t_listen    ServerMembers::getListen() const { return this->_listen; }
std::string const &ServerMembers::getLocationUri() const { return this->_locationUri; }
std::vector<Location *>& ServerMembers::getLocations() { return _locations; }
const std::vector<std::string>& ServerMembers::getServerName() { return _serverName; }
std::string const &ServerMembers::getCgiPath() const { return _cgiPath; }
ConfigMembers& ServerMembers::getConfigMembers() { return this->_members; }
std::string const &ServerMembers::getHost() { return this->_host; }

/* <------------------------------------------> */

/* <---------------> Set Area <---------------> */


void ServerMembers::setLocationUri(std::string const &uri) { this->_locationUri = uri; }

void ServerMembers::setHost(std::string const &host)
{
    _host = host;
}

void ServerMembers::setHost(unsigned int const &host)
{
    _listen.host = host;
}

void ServerMembers::setPort(int const &port)
{
    _listen.port = port;
}

void ServerMembers::setCgiPath(const std::string& path) { _cgiPath = path; }


/* <------------------------------------------> */

void ServerMembers::setServerName(const std::string& name){
    this->_serverName.push_back(name);
}

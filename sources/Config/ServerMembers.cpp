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
ConfigMembers& ServerMembers::getConfigMembers() { return this->_members; }
std::string const &ServerMembers::getHost() { return this->_host; }
std::string const &ServerMembers::getServerHeader() { return this->_serverHeader; }

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

void ServerMembers::setServerHeader(std::string const &serverHeader) { _serverHeader = serverHeader; }

/* <------------------------------------------> */

void ServerMembers::setServerName(const std::string& name){
    this->_serverName.push_back(name);
}

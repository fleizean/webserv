#include "../includes/Location.hpp"

Location::Location() {}

Location::Location(Location const &rhs) : _uri(rhs._uri) {}


Location &Location::operator=(Location const &rhs)
{
    this->_uri = rhs._uri;
    return (*this);
}

Location::~Location() {}

/* <---------------> Set Area <---------------> */

std::string const &Location::getUri() const { return this->_uri; }

std::vector<std::string> &Location::getAllowedMethods() { return _allowedMethods; }

ConfigMembers& Location::getConfigMembers() { return this->_members; }

std::map <int, std::string> Location::getReturns() { return this->_returns; }
/* <------------------------------------------> */



/* <---------------> Get Area <---------------> */

void Location::setUri(std::string const &uri) { this->_uri = uri; }

/* <------------------------------------------> */
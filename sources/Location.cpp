#include "../includes/Location.hpp"

Location::Location() :  _uri() {}

Location::Location(Location const &rhs): _uri(rhs._uri) {}

Location &Location::operator=(Location const &rhs)
{
    this->_uri = rhs._uri;

    return (*this);
}

Location::~Location() {}

/* <---------------> Set Area <---------------> */

std::string const &Location::getUri() const { return this->_uri; }

std::set<std::string> &Location::getAllowedMethods() { return this->_allowedMethods; }

ConfigMembers& Location::getConfigMembers() { return this->_members; }

/* <------------------------------------------> */



/* <---------------> Get Area <---------------> */

void Location::setUri(std::string const &uri) { this->_uri = uri; }

/* <------------------------------------------> */
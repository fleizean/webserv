#include "../../includes/Location.hpp"

Location::Location() {}

Location::Location(Location const &rhs) : _uri(rhs._uri) {}

Location &Location::operator=(Location const &rhs)
{
    this->_uri = rhs._uri;
    return (*this);
}

Location::~Location() {}

/* <---------------> Get Area <---------------> */

std::string const &Location::getUri() const { return this->_uri; }

ConfigMembers& Location::getConfigMembers() { return this->_members; }

/* <------------------------------------------> */



/* <---------------> Get Area <---------------> */

void Location::setUri(std::string const &uri) { this->_uri = uri; }

/* <------------------------------------------> */

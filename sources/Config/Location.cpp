#include "../../includes/Location.hpp"

Location::Location() : _redirectionType(0) {}

Location::Location(Location const &rhs) : _uri(rhs._uri), _redirectionType(rhs._redirectionType), _redirectionURI(rhs._redirectionURI) {}

Location &Location::operator=(Location const &rhs)
{
    this->_uri = rhs._uri;
    this->_redirectionType = rhs._redirectionType;
    this->_redirectionURI = rhs._redirectionURI;
    return (*this);
}

Location::~Location() {}

/* <---------------> Get Area <---------------> */

std::string const &Location::getUri() const { return this->_uri; }

ConfigMembers& Location::getConfigMembers() { return this->_members; }

std::string const &Location::getRedirectionURI() const { return _redirectionURI; }

unsigned int const &Location::getRedirectionType() const { return _redirectionType; }

bool const &Location::getHasRedirection() const { return _hasRedirection; }

/* <------------------------------------------> */



/* <---------------> Get Area <---------------> */

void Location::setUri(std::string const &uri) { this->_uri = uri; }

void Location::setRedirectionType(unsigned int const &redirectionType) { this->_redirectionType = redirectionType; }

void Location::setRedirectionURI(std::string const &redirectionURI) { this->_redirectionURI = redirectionURI; }

void Location::setHasRedirection(bool const &hasRedirection) { this->_hasRedirection = hasRedirection; }

/* <------------------------------------------> */

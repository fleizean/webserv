#include "Location.hpp"

Location::Location() : _allow(), _root(), _index(), _return() {}

Location::Location(Location const &rhs): _allow(rhs._allow), _root(rhs._root), _index(rhs._index), _return(rhs._return) {}

Location &Location::operator=(Location const &rhs)
{
    this->_allow = rhs._allow;
    this->_root = rhs._root;
    this->_index = rhs._index;
    this->_return = rhs._return;
}

Location::~Location() {}

/* <---------------> Set Area <---------------> */

void Location::setAllow(std::string const &allow)
{
    this->_allow = allow;
}

void Location::setRoot(std::string const &root)
{
    this->_root = root;
}

void Location::setIndex(std::string const &index)
{
    this->_index = index;
}

void Location::setReturn(std::string const &tmp_return)
{
    this->_return = tmp_return;
}
/* <------------------------------------------> */

/* <---------------> Get Area <---------------> */

std::string const &Location::getAllow() const
{
    return this->_allow;
}

std::string const &Location::getRoot() const
{
    return this->_root;
}

std::string const &Location::getIndex() const
{
    return this->_index;
}

std::string const &Location::getReturn() const
{
    return this->_return;
}

/* <------------------------------------------> */
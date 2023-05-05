#include "Location.hpp"

Location::Location() :  _root(), _allow(), _return(), _errorPage(), _maxClientBodySize(), _index() {}

Location::Location(Location const &rhs): _root(rhs._root), _allow(rhs._allow), _return(rhs._return), _errorPage(rhs._errorPage), _maxClientBodySize(rhs._maxClientBodySize), _index(rhs._index) {}

Location &Location::operator=(Location const &rhs)
{
    this->_root = rhs._root;
    this->_allow = rhs._allow;
    this->_return = rhs._return;
    this->_errorPage = rhs._errorPage;
    this->_maxClientBodySize = rhs._maxClientBodySize;
    this->_index = rhs._index;

    return (*this);
}

Location::~Location() {}

/* <---------------> Set Area <---------------> */

void Location::setRoot(std::string const &root)
{
    this->_root = root;
}

void Location::setAllow(std::string const &allow)
{
    this->_allow = allow;
}

void Location::setReturn(std::string const &returnValue)
{
    this->_return = returnValue;
}

void Location::setErrorPage(int const &errorPage)
{
    this->_errorPage = errorPage;
}

void Location::setMaxClientBodySize(std::string const &maxClientBodySize)
{
    this->_maxClientBodySize = maxClientBodySize;
}

void Location::setIndex(std::string const &index)
{
    this->_index = index;
}

/* <------------------------------------------> */

/* <---------------> Get Area <---------------> */

std::string const &Location::getRoot() const
{
    return this->_root;
}

std::string const &Location::getAllow() const
{
    return this->_allow;
}

std::string const &Location::getReturn() const
{
    return this->_return;
}

int const &Location::getErrorPage() const
{
    return this->_errorPage;
}

std::string const &Location::getMaxClientBodySize() const
{
    return this->_maxClientBodySize;
}

std::string const &Location::getIndex() const
{
    return this->_index;
}

/* <------------------------------------------> */
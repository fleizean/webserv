#include "../../includes/ConfigMembers.hpp"

ConfigMembers::ConfigMembers() : _root(""), _maxClientBodySize(1000000000), _autoIndex(false) {}

ConfigMembers::ConfigMembers(ConfigMembers const &rhs) : _root(rhs._root), _maxClientBodySize(rhs._maxClientBodySize), _autoIndex(rhs._autoIndex) {}

ConfigMembers &ConfigMembers::operator=(ConfigMembers const &rhs)
{
    this->_root = rhs._root;
    this->_maxClientBodySize = rhs._maxClientBodySize;
    this->_autoIndex = rhs._autoIndex;
    

    return (*this);
}

ConfigMembers::~ConfigMembers() {}

/* <---------------> Get Area <---------------> */

std::string const &ConfigMembers::getRoot() const { return this->_root; }

size_t const &ConfigMembers::getMaxClientBodySize() const { return this->_maxClientBodySize; }

std::map<int, std::string>& ConfigMembers::getErrorPage() { return _errorPage; }

bool const &ConfigMembers::getAutoIndex() const { return this->_autoIndex; }

std::vector<std::string>& ConfigMembers::getIndex() { return _index; }

std::map<std::string, std::string>& ConfigMembers::getCgi() { return this->_cgiPath; }

std::vector<std::string>& ConfigMembers::getAllowedMethods() { return _allowedMethods; }

/* <------------------------------------------> */


/* <---------------> Set Area <---------------> */

void ConfigMembers::setRoot(std::string const &root) { this->_root = root; }

void ConfigMembers::setMaxClientBodySize(size_t const &maxClientBodySize) { this->_maxClientBodySize = maxClientBodySize; }

void ConfigMembers::setAutoIndex(bool const &autoIndex) { this->_autoIndex = autoIndex; }



/* <------------------------------------------> */

#pragma once

#include "include.hpp"

class Location
{
    private:
        std::string _root;
        std::string _allow;
        std::string _return;
        std::string _errorPage;
        std::string _maxClientBodySize;
        std::string _index;
    public:
        Location();
        Location(Location const &rhs);
        Location &operator=(Location const &rhs);
        ~Location();

        std::string const &getRoot() const;
        std::string const &getAllow() const;
        std::string const &getReturn() const;
        std::string const &getErrorPage() const;
        std::string const &getMaxClientBodySize() const;
        std::string const &getIndex() const;

        void setRoot(std::string const &root);
        void setAllow(std::string const &allow);
        void setReturn(std::string const &returnValue);
        void setErrorPage(std::string const &errorPage);
        void setMaxClientBodySize(std::string const &maxClientBodySize);
        void setIndex(std::string const &index);

};

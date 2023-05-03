#pragma once

#include "include.hpp"

class Location
{
    private:
        std::string _allow;
        std::string _root;
        std::string _index;
        std::string _return;
    public:
        Location();
        Location(Location const &rhs);
        Location &operator=(Location const &rhs);
        ~Location();

        std::string const &getAllow() const;
        std::string const &getRoot() const;
        std::string const &getIndex() const;
        std::string const &getReturn() const;

        void setAllow(std::string const &allow);
        void setRoot(std::string const &root);
        void setIndex(std::string const &index);
        void setReturn(std::string const &tmp_return);

};
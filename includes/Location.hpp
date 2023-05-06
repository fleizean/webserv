#pragma once

#include "include.hpp"


class Location : public ConfigMembers
{
private:
    std::string             _uri;
    std::set<std::string>   _allowedMethods;
public:
    Location();
    Location(Location const &rhs);
    Location &operator=(Location const &rhs);
    ~Location();

    /* get Area */
    std::string const &getUri() const;
    std::set<std::string> &getAllowedMethods();
    /* set Area */
    void setUri(std::string const &uri);
};

#pragma once

#include "include.hpp"
#include "ConfigMembers.hpp"

class Location
{
private:
	std::string						_uri;
	std::set<std::string>			_allowedMethods;
	std::map<int, std::string>		_returns;
	ConfigMembers					_members;
public:
	Location();
	Location(Location const &rhs);
	Location &operator=(Location const &rhs);
	~Location();

	/* get Area */
	std::string const &getUri() const;
	std::set<std::string> &getAllowedMethods();
	ConfigMembers& getConfigMembers();
	std::map <int, std::string> getReturns();
	/* set Area */
	void setUri(std::string const &uri);
};

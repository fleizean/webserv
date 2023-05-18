#pragma once

#include "Include.hpp"
#include "ConfigMembers.hpp"

class Location
{
private:
	std::string						_uri;
	std::map<int, std::string>		_returns;
	ConfigMembers					_members;
public:
	Location();
	Location(Location const &rhs);
	Location &operator=(Location const &rhs);
	~Location();

	/* get Area */
	std::string const &getUri() const;
	
	ConfigMembers& getConfigMembers();
	std::map <int, std::string>& getReturns();
	/* set Area */
	void setUri(std::string const &uri);
};

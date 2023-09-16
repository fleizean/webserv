#pragma once

#include "Include.hpp"
#include "ConfigMembers.hpp"

class Location
{
private:
	std::string						_uri;
	bool							_hasRedirection;
	unsigned int					_redirectionType;
	std::string						_redirectionURI;
	ConfigMembers					_members;
public:
	Location();
	Location(Location const &rhs);
	Location &operator=(Location const &rhs);
	~Location();

	/* get Area */
	std::string const &getUri() const;
	std::string const &getRedirectionURI() const;
	unsigned int const &getRedirectionType() const;
	bool const &getHasRedirection() const;
	ConfigMembers& getConfigMembers();
	/* set Area */
	void setUri(std::string const &uri);
	void setRedirectionType(unsigned int const &redirectionType);
	void setRedirectionURI(std::string const &redirectionURI);
	void setHasRedirection(bool const &hasRedirection);
};

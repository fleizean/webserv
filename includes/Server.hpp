#pragma once

#include "include.hpp"

class Location;

class Server : public ConfigMembers
{
private:
	std::string							_host; // host
	int									_port; // port
	std::vector<Location>				_locations;
	std::vector<std::string>			_serverName;
	std::map<std::string, std::string>	_cgiPaths;
	
public:
	/* default constructor */
	Server();
	/* copy constructor */
	Server(Server const &rhs);
	/* operator assign constructor */
	Server &operator=(Server const &rhs);
	
	/* destructor */
	~Server();
	
	/* Get area */
	std::string const &getHost() const;
	int const &getPort() const;
	std::vector<Location>& getLocations();
	std::vector<std::string>& getServerName(); // & eklendi bakılacak
	std::map<std::string, std::string>& getCgiPaths(); // & eklendi bakılacak

	/* Set area */
	void setHost(std::string const &host);
	void setPort(int const &port);
};
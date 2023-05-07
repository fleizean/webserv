#pragma once

#include "include.hpp"
#include "ConfigMembers.hpp"
#include "Location.hpp"

class Server
{
private:
	std::string							_host; // host
	int									_port; // port
	vector<Location>				_locations;
	vector<std::string>			_serverName;
	map<std::string, std::string>	_cgiPaths;
	ConfigMembers						_members;
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
	ConfigMembers& getConfigMembers();
	/* Set area */
	void setHost(std::string const &host);
	void setPort(int const &port);
};
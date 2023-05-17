#pragma once

#include "Include.hpp"
#include "ConfigMembers.hpp"
#include "Location.hpp"
#include "Listen.hpp"

class Server
{
private:
	t_listen							_listen;
	std::string                         _locationUri; // location URI
	vector<Location *>					_locations;
	vector<std::string>					_serverName;
	map<std::string, std::string>		_cgiPaths;
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
	t_listen    getListen() const;
	std::string const &getLocationUri() const;
	std::vector<Location *>& getLocations();
	const std::vector<std::string>& getServerName(); // & eklendi bakılacak
	void	setServerName(const std::string& name);
	std::map<std::string, std::string>& getCgiPaths(); // & eklendi bakılacak
	ConfigMembers& getConfigMembers();
	/* Set area */
	void setHost(unsigned int const &host);
	void setPort(int const &port);
	void setLocationUri(std::string const &uri);
};

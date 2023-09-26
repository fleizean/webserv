#pragma once

#include "Include.hpp"
#include "ConfigMembers.hpp"
#include "Location.hpp"
#include "Listen.hpp"

class ServerMembers
{
private:
	std::string							_host;
	t_listen							_listen;
	std::string                         _locationUri; // location URI
	vector<Location *>					_locations;
	vector<std::string>					_serverName;
	std::string							_serverHeader;
	ConfigMembers						_members;
public:
	/* default constructor */
	ServerMembers();
	/* copy constructor */
	ServerMembers(ServerMembers const &rhs);
	/* operator assign constructor */
	ServerMembers &operator=(ServerMembers const &rhs);
	
	/* destructor */
	~ServerMembers();
	
	/* Get area */
	t_listen    getListen() const;
	std::string const &getLocationUri() const;
	std::vector<Location *>& getLocations();
	const std::vector<std::string>& getServerName(); // & eklendi bakılacak
	void	setServerName(const std::string& name);
	ConfigMembers& getConfigMembers();
	std::string  const &getHost();
	std::string const &getServerHeader();
	
	/* Set area */
	void setHost(std::string const &host);
	void setServerHeader(std::string const &serverHeader);
	void setHost(unsigned int const &host); // listen
	void setPort(int const &port); // port
	void setLocationUri(std::string const &uri);
};

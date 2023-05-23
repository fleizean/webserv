#pragma once

#include "Include.hpp"
#include "ConfigMembers.hpp"
#include "Location.hpp"
#include "Listen.hpp"

class Server
{
private:
	std::string							_host;
	t_listen							_listen;
	std::string                         _locationUri; // location URI
	vector<Location *>					_locations;
	vector<std::string>					_serverName;
	std::string							_cgiPath;
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
	std::string const &getCgiPath() const; // & eklendi bakılacak
	ConfigMembers& getConfigMembers();
	std::string  const &getHost();
	
	/* Set area */
	void setHost(std::string const &host);
	void setHost(unsigned int const &host); // listen
	void setPort(int const &port); // port
	void setLocationUri(std::string const &uri);
	void setCgiPath(std::string const &path);
};

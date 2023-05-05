#pragma once

#include "include.hpp"

/* ip ve port tutuluyor */
typedef struct	s_listen {
	unsigned int	host;
	int			port;
}				t_listen;

class Server
{
private:
    std::vector<t_listen> _listen;
    std::string _cgiPath;
    std::string _root;
    std::string _index;
    std::string _accessLog;
    std::string _errorPage;
    std::string _maxClientBodySize;
    std::string _location;

    std::map<long, Location> _locationMap;

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
    std::vector<t_listen> const &getListen() const;
    std::string const &getCgiPath() const;
    std::string const &getRoot() const;
    std::string const &getIndex() const;
    std::string const &getAccessLog() const;
    std::string const &getErrorPage() const;
    std::string const &getMaxClientBodySize() const;
    std::string const &getLocation() const;
    std::map<long, Location> const &getLocationMap() const;

    /* Set area */
    void setCgiPath(std::string const &cgiPath);
    void setRoot(std::string const &root);
    void setIndex(std::string const &index);
    void setAccessLog(std::string const &accessLog);
    void setErrorPage(std::string const &errorPage);
    void setMaxClientBodySize(std::string const &maxClientBodySize);
    void setLocation(std::string const &location);
};
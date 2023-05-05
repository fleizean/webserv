#pragma once

#include "include.hpp"

class Location;

/* ip ve port tutuluyor */
/* typedef struct	s_listen { */
/* 	unsigned int	host; */
/* 	int			port; */
/* }				t_listen; */

class Server
{
private:
    int _listen;
    unsigned int _serverName;
    std::string _cgiPath;
    std::string _root;
    std::string _index;
    std::string _accessLog;
    int _errorPage;
    size_t _maxClientBodySize;
    std::string _location;

    std::vector<Location> _locationVec;

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
    std::string const &getCgiPath() const;
    std::string const &getRoot() const;
    std::string const &getIndex() const;
    std::string const &getAccessLog() const;
    int const &getErrorPage() const;
    size_t const &getMaxClientBodySize() const;
    std::string const &getLocation() const;
    std::vector<Location>& getLocationVec();
    int const &getListen() const;
    unsigned int const &getServerName() const;

    /* Set area */
    void setCgiPath(std::string const &cgiPath);
    void setRoot(std::string const &root);
    void setIndex(std::string const &index);
    void setAccessLog(std::string const &accessLog);
    void setErrorPage(int const &errorPage);
    void setMaxClientBodySize(size_t const &maxClientBodySize);
    void setLocation(std::string const &location);
    void setListen(int const &listen);
    void setServerName(unsigned int const &serverName);
};
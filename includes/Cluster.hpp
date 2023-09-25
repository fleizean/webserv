#pragma once

#include "Include.hpp"
#include "Utils.hpp"
#include "ServerMembers.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Server;

class Cluster
{
private:    
    std::vector<ServerMembers*>     _confServers;
    std::map<int, Server *>         _servers;
    int                             _maxFd;
    fd_set                          _readFds;
    fd_set                          _supReadFds;
    fd_set                          _supWriteFds;
    fd_set                          _writeFds;
    int                             _selected;
    int                             _isMulti;
    int                             _isFav;
    int                             _loopControl;
    int                             _status;
    size_t                          _contentLen;
    std::map<int, Client *>         _clients;
    std::string                     _method;
    std::string                     _favicon;
    std::string                     _response;
    std::string                     _multiBody;
    std::string                     _body;

    Error                           err;

public:
    Cluster();
    ~Cluster();
    int     setUpCluster(std::vector<ServerMembers*> confServers);
    void    run();
    void    acceptSection();
    void    recvSection();
    void    sendSection();
    void    closeConnection(std::map<int, Client *>::iterator it);
    void	findMaxFd();
    void    cleanServers();
    void    cleanClients();
    void    cleanAll();
};
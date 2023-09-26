#pragma once

#include "Include.hpp"
#include "Utils.hpp"

class Server
{
    protected:
        int             _socketFd;
        unsigned int    _host;
        int             _port;
    
        Error           err;
    public:
        Server(unsigned int host, int port, std::map<int, int> hP);
        Server();
        Server(const Server &server);
        Server &operator=(const Server &server);
        ~Server();

        int             getFd() const;
        unsigned int    getHost() const;
        int             getPort() const;
        void            setHostPort(unsigned int host, int port);
        void            setUpServer();
        void            setUpServerMessage();
};
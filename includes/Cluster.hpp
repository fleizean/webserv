#pragma once

#include "include.hpp"
#include "Config.hpp"

class Cluster
{
public:
    Cluster(Server* server);
    Cluster(const Cluster &rhs);
    ~Cluster();

    Cluster &operator=(const Cluster &rhs);

private:
    Cluster();
    Server* server;
    std::map<long, Server>  _servers;
};
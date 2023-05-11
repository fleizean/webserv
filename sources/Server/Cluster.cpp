#include "Cluster.hpp"

Cluster::Cluster(Server* server)
{
    this->server = server;
}

Cluster::Cluster(const Cluster &rhs)
{
    if(this != &rhs)
        *this = rhs;
}

Cluster::~Cluster() { }
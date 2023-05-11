#include "../../includes/Cluster.hpp"

Cluster::Cluster(std::vector<Server*> server)
{
    this->server = server;
}

Cluster::Cluster(const Cluster &rhs)
{
    if(this != &rhs)
        *this = rhs;
}

Cluster::~Cluster() { }

std::vector<t_listen> Cluster::getAllListens() const
{
    std::vector<t_listen>	ret;
    std::vector<Server*>::const_iterator ite = server.end();
std::vector<Server*>::const_iterator it = server.begin();

    for (; it != ite; ++it)
    {
        t_listen tmp;
        tmp.host = (*it)->getHost();
        tmp.port = (*it)->getPort();
        ret.push_back(tmp);
    }
    return (ret);
}

int Cluster::setup()
{
    
    std::vector<t_listen>	vect = getAllListens(); 

    for ( std::vector<t_listen>::const_iterator lstn = vect.begin() ; lstn != vect.end() ; lstn++ )
    {
        std::cout << "Setting up " << lstn->host << ":" << lstn->port << "..." << std::endl;
    }
    return (1);
}
#pragma once

#include "include.hpp"
#include "Config.hpp"

typedef struct	s_listen {
	unsigned int	host;
	int			    port;
}				t_listen;

class Cluster
{
public:
    Cluster(std::vector<Server*> server);
    Cluster(const Cluster &rhs);
    ~Cluster();

    int		setup();
    std::vector<t_listen>	getAllListens() const;
    // Cluster &operator=(const Cluster &rhs);

private:
    Cluster();
    std::vector<Server*> server;
    std::map<long, Server>  _servers;
};
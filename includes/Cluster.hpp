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
    std::vector<Server*> server; // config
    // std::map<long, Server>  _servers;
    // std::map<long, Server *>	_sockets;
    std::vector<int>			_ready;
    fd_set						_fd_set;
	unsigned int				_fd_size;
	long						_max_fd;

};
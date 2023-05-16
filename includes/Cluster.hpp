#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "include.hpp"
#include "Listen.hpp"
#include "ClusterServer.hpp"
#include "Config.hpp"

class Cluster
{
public:
	Cluster(std::vector<Server*> server);
	Cluster(const Cluster &rhs);
	~Cluster();

	int setup();
	void run();
	void clean();
	std::vector<t_listen> getAllListens() const;
	Cluster &operator=(const Cluster &rhs);

private:
	Cluster();
	std::vector<Server*>				_conf; // config
	// std::map<long, ClusterServer>		_servers;
	// std::map<long, ClusterServer *>		_sockets;
	std::vector<int>					_ready;
	fd_set								_fd_set;
	unsigned int						_fd_size;
	long								_max_fd;

};

#endif
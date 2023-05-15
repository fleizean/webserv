#pragma once

#include "include.hpp"
#include "Config.hpp"
#include "Cluster.hpp"

class ClusterServer
{
private:
	std::map<long, std::string>     _request;
	t_listen                        _listen;
	int                            	_fd; // int yapmak zorunda kaldık bakılacak
	struct sockaddr_in              _addr;
public:
	ClusterServer(const t_listen& listen);
	ClusterServer(const ClusterServer& rhs);
	~ClusterServer();

	ClusterServer& operator=(const ClusterServer& rhs);

	/* Get Area */
	int	getFD();

	/* Functions */
	int     setup();
	void    setAddr();
	long	accept(void);
	void	process(long socket, Config &conf);
	void	processChunk(long socket);
	int		recv(long socket);
	int		send(long socket);
	void	close(int socket);
	void	clean(void);
};
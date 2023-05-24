#pragma once

#include "Include.hpp"
#include "Listen.hpp"
#include "ServerMembers.hpp"
#include "Location.hpp"
#include <arpa/inet.h>


#define MAX_CONNECTIONS 65535 
#define DATA_BUFFER 800000


class Server
{
private:
	int					fd[MAX_CONNECTIONS];
	int					efd;
	// int					sockfd;
	int					NbPort;
	struct sockaddr_in	new_addr;
	int all_connections[MAX_CONNECTIONS];

	std::string buffu;

	std::vector<ServerMembers*> _servers;


	std::string _host;
	int _port;
public:
	Server(std::vector<ServerMembers*> server);
	~Server();

	int run();
	int getNbPort();
	void setNbPort(int n);

	int create_server(int iport, std::string host);
	void create_socket(int port, std::string host);
	int read_connection(int socket);
	void setup(); 
};
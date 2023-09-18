#pragma once

#include "Include.hpp"
#include "Listen.hpp"
#include "ServerMembers.hpp"
#include "Location.hpp"
#include <arpa/inet.h>
#include "Request.hpp"
#include "Response.hpp"

#define MAX_CONNECTIONS 65535 
#define DATA_BUFFER 800000

class Server
{
private:
	int					fd[MAX_CONNECTIONS];
	int					efd;
	int 				all_connections[MAX_CONNECTIONS];
	// int					sockfd;
	int					NbPort;
	std::string 		_host;
	std::string 		buffer;
	std::string			_errorResponse;
	bool				_foundError;
	int					_time;
	int _port;

	socklen_t addrlen;
	struct sockaddr_in	new_addr;

	std::vector<ServerMembers*> _servers;
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


	bool checkValidSockets();
	void closeConnection(int &connectionIndex, fd_set& read_fd_set);

	void selectConnection(fd_set& read_fd_set, int& new_fd);
	void processActiveConnection(int connectionIndex, fd_set& read_fd_set);
	void handleConnectionError(int connectionIndex);
	void processActiveConnections(fd_set& read_fd_set);
	bool validateLocationUri();

	ServerMembers*    getServerForRequest(t_listen& address, std::vector<ServerMembers*>& servers);
};
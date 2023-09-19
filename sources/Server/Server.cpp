#include "../../includes/Server.hpp"
#include <chrono>
#include <thread>


/* ------------> CANNONICAL FUNCTIONS <------------ */

Server::Server(std::vector<ServerMembers*> server) 
{ 
	_servers = server;
	setup(); // setup server
}

Server::~Server() 
{
	for (int i = 0; i < getNbPort(); i++)
	{
		close(fd[i]);
	}
	close(efd);
}

/* ------------> GET FUNCTIONS <-------------- */

int Server::getNbPort() { return this->NbPort; }

/* ------------> SET FUNCTIONS <-------------- */

void Server::setNbPort(int port) { this->NbPort = port; }

/* ------------> OTHERS FUNCTIONS <-------------- */

void Server::create_socket(int port, std::string host)
{
	Error err(0);
	struct sockaddr_in socket_addr;
	int  ret_val;

	(void)host;

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(port);
	socket_addr.sin_addr.s_addr = htonl(INADDR_ANY); // hata 2
	//inet_pton(AF_INET, host.c_str(), &(socket_addr.sin_addr));
	ret_val = ::bind(efd, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr_in));
	if (ret_val != 0)
	{
		close(efd);
		err.setAndPrint(39, "Server::create_socket");
	}
	ret_val = listen(efd, 128);
	if (ret_val != 0)
	{
		close(efd);
		err.setAndPrint(40, "Server::create_socket");
	}
}

int Server::create_server(int port, std::string host)
{
	Error err(0);
	int	opt_val = 1;

	efd = socket(AF_INET, SOCK_STREAM, 0);
	if (efd == -1)
		err.setAndPrint(36, "Server::create_server");
	
	if (fcntl(efd, F_SETFL, O_NONBLOCK) < 0)
		err.setAndPrint(37, "Server::create_server");
	
	if (setsockopt(efd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val)) == -1)
		err.setAndPrint(38, "Server::create_server");
	create_socket(port, host);
	return efd;
}

int Server::read_connection(int socket)
{
	char	tmp_buffer[DATA_BUFFER + 1];
	
	int ret = recv(socket, tmp_buffer, DATA_BUFFER, 0);
	if (ret == -1)
		return -1;
	if (ret == 0)
        return 5;
	_buffer += std::string(tmp_buffer, ret);
	size_t res = _buffer.find("\r\n\r\n");
	if (res != std::string::npos)
	{
		if (_buffer.find("Content-Length: ") == std::string::npos)
			return (0);
	
		size_t len = std::atoi(_buffer.substr(_buffer.find("Content-Length: ") + strlen("Content-Length: "), 10).c_str());
		
		if (_buffer.size() >= len + res + strlen("\r\n\r\n"))
			return (0);
		else
			return (1);
	}
	return (1);
}

void Server::setup()
{
	setNbPort(_servers.size());
	std::vector<ServerMembers*>::const_iterator ite = _servers.end();
    std::vector<ServerMembers*>::const_iterator it = _servers.begin();
	std::map<std::string, int> hP;
	bool flag;
	for (int i = 0; it != ite; ++it, i++)
    {
		flag = false;
		_port = (*it)->getListen().port;
		_host = (*it)->getHost();
        std::cout << BOLD_CYAN << _host << ":" << _port << " setuping and listening right now..." << RESET << "\n";
	 	int p = 0;
		for (std::map<std::string, int>::iterator it = hP.begin(); it != hP.end(); ++it, p++) 
		{
			if (it->first == _host && it->second == _port)
			{
		    	fd[i] = fd[p];
				flag = true;
				break;
			}
		}
		if(flag == false)
			fd[i] = create_server(_port, _host);
		hP[_host] = _port;
	}
}

/* utils functions */

bool Server::checkValidSockets()
{
	for (int i = 0; i < getNbPort(); i++)
	{
		if (fd[i] == -1)
		{
			return false;
		}
	}

	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		all_connections[i] = -1;
	}

	for (int i = 0; i < getNbPort(); i++)
	{
		all_connections[i] = fd[i];
	}
	return true;
}

void Server::selectConnection(fd_set& read_fd_set, int& new_fd)
{
	for (int i = 0; i < getNbPort(); i++)
	{
		if (FD_ISSET(fd[i], &read_fd_set))
		{
			new_fd = accept(fd[i], (struct sockaddr *)&new_addr, &addrlen);
			fcntl(fd[i], F_SETFD, FD_CLOEXEC);
			if (new_fd >= 0)
			{
				for (i = 0; i < MAX_CONNECTIONS; i++)
				{
					if (all_connections[i] < 0)
					{
						all_connections[i] = new_fd;
						break;
					}
				}
			}
			if (new_fd == -1)
			{
				std::cout << "accept() failed for fd \n"
						  << strerror(errno) << std::endl;
			}

		}
	}
}

void Server::processActiveConnection(int connectionIndex, fd_set& read_fd_set)
{
	
	ServerMembers* matchedServer;
    char tmp_buff[DATA_BUFFER + 1];
	strcpy(tmp_buff, _buffer.c_str());
    Request pr(tmp_buff); // Requst Parser
    matchedServer = getServerForRequest(pr.getListen(), _servers, pr); // matchedServer -> verdiğim ip ve port ile eşleşen serverı getirir
	if (matchedServer == NULL) 
	{
		std::string _responseHeader;
		_responseHeader += "HTTP/1.1 404 OK" ;
		_responseHeader += "\nContent-Type: text/html" ;
		_responseHeader += "\n\n";
		_responseHeader += "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n</head>\n<h1>404 Error.</h1></html>";
		write(all_connections[connectionIndex], _responseHeader.c_str(), _responseHeader.size() + 1);
	}
	else
	{
		Response response(pr, _servers, matchedServer); //
		response.setBando(_buffer);
		response.checkModifyDate();
		response.setDate();
		response.errorStatus();

		response.run();

		write(all_connections[connectionIndex], response.getResponseHeader().c_str(), response.getResponseHeader().size() + 1);
	}
	/* closing area */
	closeConnection(connectionIndex, read_fd_set);
}

void Server::processActiveConnections(fd_set& read_fd_set)
{
	int error_handle = -1;
    for (int i = 1; i < MAX_CONNECTIONS; i++)
    {
        if ((all_connections[i] > 0) && (FD_ISSET(all_connections[i], &read_fd_set)))
        {
            error_handle = read_connection(all_connections[i]);
            if (error_handle == 0)
            {
                processActiveConnection(i, read_fd_set);
                break;
            }
			else if (error_handle == 5)
			{
				close(all_connections[i]);
				all_connections[i] = -1;
			}
			else if (error_handle == -1)
            {
                break;
            }
        }
    }
}

void Server::closeConnection(int &connectionIndex, fd_set& read_fd_set)
{
	close(all_connections[connectionIndex]);
    fcntl(all_connections[connectionIndex], F_SETFD, FD_CLOEXEC);
    FD_CLR(all_connections[connectionIndex], &read_fd_set);
    all_connections[connectionIndex] = -1; 
}

int Server::run()
{
	_time = 0;
    Error err(0);
    fd_set read_fd_set;
	fd_set write_fd_set = {0};
	int new_fd, ret_val;

	if (!checkValidSockets()) // socket validasyon kontrolü
        err.setAndPrint(52, "Server::run");
	while (1)
	{
		FD_ZERO(&read_fd_set);
		FD_ZERO(&write_fd_set);
		for (int i = 0; i < MAX_CONNECTIONS; i++)
		{
			if (all_connections[i] >= 0)
			{
				FD_SET(all_connections[i], &read_fd_set);
			}
		}
		ret_val = select(FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);
		if (ret_val >= 0)
		{
			selectConnection(read_fd_set, new_fd);
			processActiveConnections(read_fd_set);
		}
    }

    close(new_fd);
    close(ret_val);
    return 0;
}

ServerMembers*    Server::getServerForRequest(t_listen& address, std::vector<ServerMembers*>& servers, Request &req)
{	
	
	for (std::vector<ServerMembers *>::const_iterator it = servers.begin() ; it != servers.end(); it++)
    {
		address.host = 2130706433;
		if (!(*it)->getServerName().empty())
		{
			for (std::vector<std::string>::const_iterator sit = (*it)->getServerName().begin(); sit != (*it)->getServerName().end(); sit++)
			{
				if (*sit == req.getHost() && address.host == (*it)->getListen().host && address.port == (*it)->getListen().port)
				{
					return (*it);
				}
			}
		}
		else 
		{
			if (address.host == (*it)->getListen().host && address.port == (*it)->getListen().port)
        	{
        	    return (*it);
        	}
		}
        
    }
    // Boyle bir durum soz konusu degil ama yinede NULL donduruyoruz
    return NULL;
}
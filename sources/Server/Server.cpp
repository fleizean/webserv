#include "../../includes/Server.hpp"

/* ------------> CANNONICAL FUNCTIONS <------------ */

Server::Server(std::vector<ServerMembers*> server, char **env) 
{ 
	Error err(0);
	_servers = server;
	_env = env;
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

	socket_addr.sin_family = AF_INET;
	socket_addr.sin_port = htons(port);
	inet_pton(AF_INET, host.c_str(), &(socket_addr.sin_addr));
	ret_val = ::bind(efd, (struct sockaddr *)&socket_addr, sizeof(struct sockaddr_in));
	if (ret_val != 0)
	{
		close(efd);
		err.setAndPrint(39, "Server::create_socket");
	}
	ret_val = listen(efd, 4096);
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
    Error err(0);
	char	tmp_buffer[DATA_BUFFER + 1];

	memset(tmp_buffer, 0, DATA_BUFFER);
	int ret = recv(socket, tmp_buffer, DATA_BUFFER, 0);
	if (ret == -1){
		return -1;
	}
	else if (ret == 0)
        return 5;
	buffer += std::string(tmp_buffer, ret);
	size_t res = buffer.find("\r\n\r\n");
	if (res != std::string::npos)
	{
		if (buffer.find("Content-Length: ") == std::string::npos)
			return (0);
	
		size_t len = std::atoi(buffer.substr(buffer.find("Content-Length: ") + strlen("Content-Length: "), 10).c_str());
		
		if (buffer.size() >= len + res + strlen("\r\n\r\n"))
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

	for (int i = 0; it != ite; ++it, i++)
    {
		_port = (*it)->getListen().port;
		_host = (*it)->getHost();
        std::cout << BOLD_CYAN << _host << ":" << _port << " setuping and listening right now..." << RESET << "\n";
		fd[i] = create_server(_port, _host);
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

void Server::selectConnection(int& ret_val, fd_set& read_fd_set, int& new_fd)
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
			ret_val--;
			if (!ret_val)
				continue;
		}
	}
}

void Server::processActiveConnection(int connectionIndex, fd_set& read_fd_set)
{
	// Location* matchedLocation;
	//ServerMembers* matchedServer;
    char tmp_buff[DATA_BUFFER + 1];
    memset(tmp_buff, 0, sizeof(tmp_buff));
    strcpy(tmp_buff, buffer.c_str());
	// (void)matchedLocation;
    Request pr(tmp_buff);
    // matchedServer = getServerForRequest(pr.getListen(), _servers);
	// matchedLocation = getLocationForRequest(matchedServer, pr.getLocation());
	// std::cout << "dönen server: " << matchedServer->getConfigMembers().getRoot() << std::endl;
	// std::cout << "dönen location: " << matchedLocation->getUri() << std::endl;
	Response response(pr, _servers, _env);
	/* if (_foundError == true && pr.getLocation() != "/"){
		setErrorPage(pr);
		write(all_connections[connectionIndex], _errorResponse.c_str(), _errorResponse.size() + 1);
		_foundError = false;
	}
	else */
	response.setBando(buffer);
	response.checkModifyDate();
	response.setDate();
	response.errorStatus();

	response.run();
	write(all_connections[connectionIndex], response.getResponseHeader().c_str(), response.getResponseHeader().size() + 1);
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
	buffer = "";
	close(all_connections[connectionIndex]);
    fcntl(all_connections[connectionIndex], F_SETFD, FD_CLOEXEC);
    FD_CLR(all_connections[connectionIndex], &read_fd_set);
    all_connections[connectionIndex] = -1; 
}

int Server::run()
{
    Error err(0);
    fd_set read_fd_set;
	fd_set write_fd_set = {0};
	int new_fd, ret_val;
	
	std::string kfe;

	if (!checkValidSockets()) // socket validasyon kontrolü
        return -1;
	
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
		if(ret_val >= 0)
		{
			selectConnection(ret_val, read_fd_set, new_fd);
			processActiveConnections(read_fd_set);
		}
    }

    close(new_fd);
    close(ret_val);
    return 0;
}

ServerMembers*    Server::getServerForRequest(t_listen& address, std::vector<ServerMembers*>& servers)
{	
	for (std::vector<ServerMembers *>::const_iterator it = servers.begin() ; it != servers.end(); it++)
    {
        if (address.host == (*it)->getListen().host && address.port == (*it)->getListen().port)
        {
            return (*it);
        }
    }
    // Boyle bir durum soz konusu degil ama yinede NULL donduruyoruz
    return NULL;
}

Location* Server::getLocationForRequest(ServerMembers* server, const std::string& uri)
{
	_foundError = true;
	for (std::vector<Location *>::iterator it = server->getLocations().begin(); it != server->getLocations().end(); it++)
	{
		if ((*it)->getUri() == uri)
        {
			_foundError = false;
            return (*it);
        }
	}
	// Boyle bir durum soz konusu degil ama yinede NULL donduruyoruz
	return NULL;
}

void Server::setErrorPage(Request pr)
{
	_errorResponse = pr.getProtocol() + " 404 Not Found\r\n";
	_errorResponse += "Content-Type: text/html\r\n";
	_errorResponse += "Connection: close\r\n";
	_errorResponse += "\r\n";
	_errorResponse += "<!DOCTYPE html>\n<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1><p>There was an error finding your error page</p></body></html>\n";
}
#include "../../includes/Server.hpp"

Server::Server(unsigned int host, int port, std::map<int, int> hP)
{
    bool flag = false;
    setHostPort(host, port);
	
	
	for (std::map<int, int>::iterator it = hP.begin(); it != hP.end(); ++it) 
	{
		if (it->first == _port)
		{
	    	_socketFd = it->second;
			flag = true;
			break;
		}
	}
	if(flag == false)
        	setUpServer();
	setUpServerMessage();    
}

Server::Server() {}

Server::~Server() {}

Server& Server::operator=(const Server &server)
{
	if(this == &server)
		return(*this);
	this->_socketFd = server._socketFd;
	this->_port = server._port;
	this->_host = server._host;
	return (*this);
}

void	Server::setUpServer()
{
	struct sockaddr_in srvaddr;
	int opt = 1;
	this->_socketFd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(this->_socketFd == -1)
		err.setAndPrint(30,"Server::setUpServer");
	
	fcntl(this->_socketFd, F_SETFL, O_NONBLOCK);
	
	if(setsockopt(this->_socketFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(int)) < 0)
	{	
		close(this->_socketFd);
		err.setAndPrint(38, "Server::setUpServer");
	}
	
/* 	memset((char*)&srvaddr, 0, sizeof(srvaddr)); */
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(this->_port);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(::bind(this->_socketFd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		close(this->_socketFd);
		err.setAndPrint(39, "Server::setUpServer");
	}
	if(listen(this->_socketFd, 128) < 0)
	{
		close(this->_socketFd);
		err.setAndPrint(40, "Server::setUpServer");
	}
}

void	Server::setUpServerMessage()
{
	std::cout << BOLD_CYAN << _host << ":" << _port << " setuping and listening right now..." << RESET << "\n";
}


Server::Server(const Server &server) 
{
	*this = server;
}


void Server::setHostPort(unsigned int host, int port)
{
	this->_host = host;
	this->_port = port;
}

int Server::getFd(void)	const
{
	return (_socketFd);
}

int Server::getPort(void) const
{
    return (this->_port);
}

unsigned int Server::getHost(void) const
{
    return (this->_host);
}
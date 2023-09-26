#include "../../includes/Cluster.hpp"
#

Cluster::Cluster() {}

Cluster::~Cluster() {}

int Cluster::setUpCluster(std::vector<ServerMembers*> confServers)
{
    Server *server = NULL;
    Error err(0);
    int socket = -1;
    _loopControl = 1;
    _confServers = confServers;
    _status = 0;
    _isMulti = 0;
    _contentLen = 0;
    _response = "";
    _method = "";
    _body = "";
    _multiBody = "";
    
    _favicon = add_headers_favicon(openNread(getPwd() + "/" + "favicon/favicon.ico")); 
    
    FD_ZERO(&this->_writeFds);
	FD_ZERO(&this->_readFds);
	FD_ZERO(&this->_supReadFds);
	FD_ZERO(&this->_supWriteFds);
    _maxFd = 0;
	_selected = 0;

    std::vector<ServerMembers*>::const_iterator ite = _confServers.end();
    std::vector<ServerMembers*>::const_iterator it = _confServers.begin();

    for (int i = 0; it != ite; ++it, i++) // buraya bakılacak
    {
        server = new Server((*it)->getListen().host, (*it)->getListen().port);
        socket = server->getFd();
        FD_SET(socket, &_readFds);
        _servers.insert(std::pair<int, Server*>(socket, server));
        _maxFd = socket + 1;
    }

    if (_maxFd == 0)
    {
        err.setAndPrint(29, "Cluster::setUpCluster");
        return -1;
    }
    return 0;
}

void Cluster::acceptSection()
{
    int client_fd;
    Client *client;
    for(std::map<int, Server *>::iterator it = _servers.begin(); this->_loopControl &&  it != _servers.end(); it++)
    {
        if(FD_ISSET(it->first, &_supReadFds))
        {
			std::cout << YELLOW << "\nAccepting..." << RESET << std::endl;

            client_fd = accept(it->first, NULL, NULL);
            if(client_fd == -1)
            {
                //***********err.setAndPrint();
                return;
            }
            if(client_fd != -1)
            {
                FD_SET(client_fd, &_readFds);
                client = new Client(it->second, this->_confServers);
              	this->_clients.insert(std::pair<int, Client *>(client_fd, client));
				findMaxFd();
			}
			this->_loopControl = 0;
			break;
		}
	}
}


void Cluster::sendSection()
{
    long sent;
    for (std::map<int, Client *>::iterator it = _clients.begin(); this->_loopControl && it != _clients.end(); it++)
    {
        if (FD_ISSET(it->first, &_supWriteFds))
        {
            _response = it->second->process(this->_multiBody);
            sent = send(it->first, _response.c_str(), _response.length(), 0);
            if (sent > 0)
				std::cout << CYAN << "Send Successful!" << RESET << std::endl << std::flush;
            closeConnection(it);
            this->_loopControl = 0;
            break;
        }
    }
}

/* void Cluster::recvSection()
{
    int ret;
    char buffer[4096] = {0};
    size_t supLen = 0;

    ret = -1;
    for (std::map<int, Client *>::iterator it = _clients.begin(); this->_loopControl && it != _clients.end(); it++)
    {
        if(FD_ISSET(it->first, &_supReadFds))
        {
            std::cout << CYAN << "\nReceiving..." << RESET << std::endl;
            ret = recv(it->first, buffer, 4095, 0);
            if(ret > 0)
            {
                if(this->_status == 0)
                {
                    it->second->setParserRequest(buffer);
                    this->_status = it->second->getStatus();
					this->_isMulti = it->second->getMulti();
					this->_method = it->second->getMethod();
					this->_isFav = it->second->getIsFav();
					this->_contentLen = it->second->getContentLen();
					this->_body = it->second->getBody();
                    if(this->_isFav == 1)
                    {
                        if(send(it->first, this->_favicon.c_str(), this->_favicon.length(), 0) > 0)
                        	std::cout << CYAN << "Send Successful Favicon!" << RESET << std::endl << std::flush;
                        this->_loopControl = 0;
                        closeConnection(it);
                        break;
                    }
                    else if(this->_status == 1)
                    {
                        if(this->_isMulti == 1)
                        {
                            if(static_cast<size_t>(ret) >= this->_contentLen)
                            {
                                FD_CLR(it->first, &this->_readFds);
                                FD_SET(it->first, &this->_writeFds);
                                this->_multiBody += std::string(buffer, ret);
                                this->_multiBody = this->_multiBody.substr(this->_multiBody.find("------Web"));
                            }
                            else if(this->_body != "")
                            {
                                this->_multiBody += std::string(buffer, ret);
                                this->_multiBody = this->_multiBody.substr(this->_multiBody.find("------Web"));
                            }
                            this->_body = "";
                        }
                        else
                        {
                            FD_CLR(it->first, &this->_readFds);
                            FD_CLR(it->first, &this->_writeFds);
                        }
                    }
                    else
                    {
                        closeConnection(it);
                        err.setAndPrint(53, "Cluster::recvSection");
                    }
                }
                else if(this->_status == 1 && this->_isMulti == 1)
                {
                    supLen = this->_multiBody.length();
                    if(!(supLen + ret == std::string::npos))
                    {
                        this->_multiBody += std::string(buffer, ret);
                        if(this->_contentLen == this->_multiBody.length())
                        {
                            FD_CLR(it->first, &this->_readFds);
                            FD_SET(it->first, &this->_writeFds);
                        }
                    }
                    else
                    {
                        closeConnection(it);
                        err.setAndPrint(54, "Cluster::recvSection");
                    }
                }
            }
            else
            {
                if(ret == 0)
                		std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
                else if(ret == -1){
                    
                }
                    closeConnection(it);

				std::cout << "Connection Closed!" << std::endl << std::flush;
            }
            this->_loopControl = 0;
            break;
        }
    }
} */


void Cluster::recvSection()
{
	int	ret;
	char buffer[4096] = {0};
	size_t	sup_len = 0;

	ret = -1;
	for (std::map<int, Client *>::iterator it = _clients.begin(); this->_loopControl && it != _clients.end(); it++)
	{
		if (FD_ISSET(it->first, &_supReadFds))
		{
    		std::cout << YELLOW <<  "\nReceiving..." << RESET << std::endl;
			ret = recv(it->first, buffer, 4095, 0);
			if (ret > 0)
			{
				if (this->_status == 0)
				{
					it->second->setParserRequest(buffer);
					this->_status = it->second->getStatus();
					this->_isMulti = it->second->getMulti();
					this->_method = it->second->getMethod();
					this->_isFav = it->second->getIsFav();
					this->_contentLen = it->second->getContentLen();
					this->_body = it->second->getBody();
					if (this->_isFav == 1)
					{
						if (send(it->first, this->_favicon.c_str(), this->_favicon.length(), 0) > 0)
							std::cout << CYAN << "Send Successful Favicon!" << RESET << std::endl << std::flush;
						this->_loopControl = 0;
						closeConnection(it);
						break ;
					}
					else if (this->_status == 1)
					{
						if (this->_isMulti == 1)
						{
							if (static_cast<size_t>(ret) >= this->_contentLen)
							{
								FD_CLR(it->first, &this->_readFds);
								FD_SET(it->first, &this->_writeFds);
								this->_multiBody += std::string(buffer, ret);
								this->_multiBody = this->_multiBody.substr(this->_multiBody.find("------Web"));
							}
							else if (this->_body != "")
							{
								this->_multiBody += std::string(buffer, ret);
								this->_multiBody = this->_multiBody.substr(this->_multiBody.find("------Web"));
							}
							this->_body = "";
						}
						else
						{
							FD_CLR(it->first, &this->_readFds);
							FD_SET(it->first, &this->_writeFds);
						}
					}
					else
					{
						closeConnection(it);
						// _clusterException.run(401);
					}
				}
				else if (this->_status == 1 && this->_isMulti == 1)
				{
					sup_len = this->_multiBody.length();
					if (!(sup_len + ret == std::string::npos))
					{
						this->_multiBody += std::string(buffer, ret);
						if (this->_contentLen == this->_multiBody.length())
						{
							FD_CLR(it->first, &this->_readFds);
							FD_SET(it->first, &this->_writeFds);
						}
					}
					else
					{
						// _clusterException.run(402);
						closeConnection(it);
					}
				}
			}
			else
			{
				if (ret == 0)
				{
            		std::cout << RED << "\rConnection was closed by client.\n" << RESET << std::endl;
				}
        		else if (ret == -1)
				{
                    return;
					// _clusterException.run(403);
				}
				closeConnection(it);
				std::cout << "Connection Closed!" << std::endl << std::flush;
			}
			this->_loopControl = 0;
			break;
		}
	}
}

void Cluster::run()
{
    while(1)
    {
        this->_selected = 0;
        while(_selected == 0)
        {
            FD_ZERO(&_supReadFds);
            FD_ZERO(&_supWriteFds);
            this->_supWriteFds = this->_writeFds;
            this->_supReadFds = this->_readFds;
            this->_selected = select(_maxFd, &this->_supReadFds, &this->_supWriteFds, NULL, NULL);
        }
        if(_selected > 0)
        {
            this->_loopControl = 1;
            recvSection();
            sendSection();
            acceptSection();
        }
        else
        {
            std::cerr << RED << "\nSelect Error code: " << errno << "\nError message: " << strerror(errno) << RESET << std::endl;
            cleanAll();
        }
    }
}

void	Cluster::findMaxFd()
{
	this->_maxFd = -1;

	for(std::map<int, Server *>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		if (it->first >= this->_maxFd)
		{
			this->_maxFd = it->first;
			this->_maxFd += 1;
		}
	}

	for(std::map<int, Client *>::iterator iter = this->_clients.begin(); this->_selected && iter != this->_clients.end(); iter++)
	{
		if (iter->first >= this->_maxFd)
		{
			this->_maxFd = iter->first;
			this->_maxFd += 1;
		}
	}
}

void Cluster::cleanServers()
{
	for (std::map<int, Server *>::iterator it = _servers.begin(); it != _servers.end(); it++)
	{
		close(it->first);
		delete(it->second);
	}
	_servers.clear();
}

void Cluster::cleanClients()
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		close(it->first);
		delete(it->second);
	}
	_clients.clear();
}

void Cluster::cleanAll()
{
	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);
	FD_ZERO(&_supReadFds);
	FD_ZERO(&_supWriteFds);
	this->cleanServers();
	this->cleanClients();
}

void	Cluster::closeConnection(std::map<int, Client *>::iterator it)
{
	this->_status = 0;
	this->_isMulti = 0;
	this->_isFav = 0;
	this->_method = "";
	this->_contentLen = 0;
	this->_body = "";
	this->_multiBody = "";
	FD_CLR(it->first, &this->_readFds);
	FD_CLR(it->first, &this->_writeFds);
	close(it->first);
	delete(it->second);
	this->_clients.erase(it);
	this->findMaxFd();
}
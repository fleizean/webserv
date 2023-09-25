#include "../../includes/Cluster.hpp"

Cluster::Cluster() {}

Cluster::~Cluster() {}

int Cluster::setUpCluster(std::vector<ServerMembers*> confServers)
{
    // Server *server = NULL;
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

    for (int i = 0; it != ite; ++it, i++)
    {
        /* server = new Server((*it)->getHost(), (*it)->getListen().port) */
        /* socket = server->getFd(); */
        FD_SET(socket, &_readFds);
        /* _servers.insert(std::pair(int, Server *>(socket, server))); */
        _maxFd = socket + 1;
    }

    if (_maxFd == 0)
    {
        err.setAndPrint(29, "Cluster::setUpCluster");
        return -1;
    }
    return 0;
}

void Cluster::recvSection()
{
    int ret;
    char buffer[4096] = {0};
    size_t sup_len = 0;

    ret = -1;
    for (std::map<int, Client *>::iterator it = clients.begin(); this->_loopControl && it != clients.end(); it++)
    {
        if(FD_ISSET(it.first, &_supReadFds))
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
                    if(this->isFav == 1)
                    {
                        if(send(it->first, this->_favicon))
                    }
                }
            }
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
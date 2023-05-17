#include "../../includes/ClusterServer.hpp"

ClusterServer::ClusterServer(const t_listen& listen) 
{ 
	_listen = listen;
	_fd = -1;
	this->setAddr();
}

ClusterServer::ClusterServer(const ClusterServer& rhs)
{
	*this = rhs;
}

ClusterServer &ClusterServer::operator=(const ClusterServer &rhs)
{
	_listen = rhs._listen;
	_fd = rhs._fd;
	_addr = rhs._addr;
	return (*this);
}

ClusterServer::~ClusterServer() { }

/* get area */
int ClusterServer::getFD() { return _fd; } // int yapmak zorunda kaldık bakılacak

/* func */
int ClusterServer::setup()
{
	Error err(0);

	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1)
		err.setAndPrint(30, "ClusterServer::setup");
	this->setAddr();
	if (::bind(_fd, (struct sockaddr *) &_addr, sizeof(_addr)) == -1) // hata bulunamadı
		err.setAndPrint(31, "ClusterServer::setup");
	if (listen(_fd, 1000) == -1)
		err.setAndPrint(32, "ClusterServer::setup");
	return (0);
}

void ClusterServer::setAddr(void)
{
	memset((char *)&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = htonl(_listen.host);
	_addr.sin_port = htons(_listen.port);
}

long ClusterServer::accept()
{
	Error err(0);
	long	socket;

	socket = ::accept(_fd, NULL, NULL);
	if (socket == -1)
		err.setAndPrint(33, "ClusterServer::accept");
	else
	{
		fcntl(socket, F_SETFL, O_NONBLOCK);
		_request.insert(std::make_pair(socket, ""));
	}
	return (socket);
}

/* void ClusterServer::process(long socket, std::vector<Server*> conf)
{
	Response response;
	Server *matchedServer;
	Location *matchedLocation;

	if (_request[socket].find("Transfer-Encoding: chunked") != std::string::npos && _request[socket].find("Transfer-Encoding: chunked") < _request[socket].find("\r\n\r\n"))
        this->processChunk(socket); // processChunk daha yok

	if (_request[socket].size() < 1000)
        std::cout << "\nRequest :" << std::endl
                  << "[" << _request[socket] << "]" << std::endl;
    else
        std::cout << "\nRequest :" << std::endl
                  << "[" << _request[socket].substr(0, 1000) << "..." << _request[socket].substr(_request[socket].size() - 10, 15) << "]" << std::endl;
	
	if (_request[socket] != "")
    {
        Request *request;
        ParserRequest parserRequest(_request[socket]);// aldığımız isteği parçalamak üzere Request class'a gönderiyoruz.

        parserRequest.parse();
        request = parserRequest.getRequest();

        matchedServer = this->getServerForRequest(this->_listen, request->getIp(), conf);
        matchedLocation = this->getLocationForRequest(matchedServer, request->getPath());

        response.createResponse(request, matchedServer, matchedLocation);

        // socket,request olan map yapısının requestini siliyoruz
        _request.erase(socket);
        // requeste cevap oluşturup map içinde socket,response şeklinde tutuyoruz.
        _request.insert(std::make_pair(socket, response.getResponse()));

        //not:
        //www.google.com:80
        //192.282.23.2:
        //192.80.808.1:8080
    }
} */

void		ClusterServer::close(int socket)
{
	if (socket > 0)
		::close(socket);
	_request.erase(socket);
}

void		ClusterServer::clean(void)
{
	if (_fd > 0)
		::close(_fd);
	_fd = -1;
}

/****************************** GET SERVER AND LOCATION ******************************/

Server*    ClusterServer::getServerForRequest(t_listen& address, const std::string& hostname, std::vector<Server*> conf)
{
	Error err(0);
	std::vector<Server *>  matchingServers;

	std::vector<Server*>::const_iterator ite = conf.end();
    std::vector<Server*>::const_iterator it = conf.begin();
	
	for (; it != ite; ++it)
	{
		if(address.host == (*it)->getListen().host && address.port == (*it)->getListen().port)
		{
			matchingServers.push_back(*it);
			for(size_t i = 0; i < (*it)->getServerName().size(); i++)
            {
                if((*it)->getServerName().at(i) == hostname)
                    return *it;
            }
		}
	}
	if (matchingServers.empty())
		err.setAndPrint(34, "ClusterServer::getServerForRequest");
    // If no server name matches, return the first matching server
    return matchingServers.front();
}

Location*  ClusterServer::getLocationForRequest(Server *matchedServer, const std::string& path)
{
	const std::vector<Location *> &locations = matchedServer->getLocations();

    for(std::vector<Location *>::const_iterator it = locations.begin(); it != locations.end(); it++)
    {
        if(path == (*it)->getUri())
            return (*it);
    }
    return (*(matchedServer->getLocations().begin()));
} 


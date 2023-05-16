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

/* void ClusterServer::process(long socket, Config &conf)
{

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

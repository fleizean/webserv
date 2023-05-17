#include "../../includes/Cluster.hpp"

Cluster::Cluster(std::vector<Server*> conf)
{
    this->_conf = conf;
}

Cluster::Cluster(const Cluster &rhs)
{
    if(this != &rhs)
        *this = rhs;
}

Cluster &Cluster::operator=(const Cluster& rhs)
{
    _conf = rhs._conf;
    //_servers = rhs._servers; açılacak
    _fd_set = rhs._fd_set;
    _fd_size = rhs._fd_size;
    _max_fd = rhs._max_fd;
    return (*this);
}

Cluster::~Cluster() { }

std::vector<t_listen> Cluster::getAllListens() const
{
    std::vector<t_listen>	ret;
    std::vector<Server*>::const_iterator ite = _conf.end();
    std::vector<Server*>::const_iterator it = _conf.begin();

    for (; it != ite; ++it)
    {
        t_listen tmp;
        tmp.host = (*it)->getListen().host;
        tmp.port = (*it)->getListen().port;
        ret.push_back(tmp);
    }
    return (ret);
}

int Cluster::setup()
{
    Error err(0);
    std::vector<t_listen>	vect = getAllListens(); 

    /* for ( std::vector<t_listen>::const_iterator lstn = vect.begin() ; lstn != vect.end() ; lstn++ )
    {
        std::cout << "Setting up " << lstn->host << ":" << lstn->port << "..." << std::endl;
    } */
    /* FD_ZERO(&_fd_set);
    _fd_size = vect.size();
	_max_fd = 0;
    for ( std::vector<t_listen>::const_iterator lstn = vect.begin() ; lstn != vect.end() ; lstn++ )
	{
		Server		serv(*lstn); // bakılacak
		long		fd;

		if (serv.setup() != -1)
		{
			fd = serv.getFD();
			FD_SET(fd, &_fd_set);
			_servers.insert(std::make_pair(fd, serv));
			if (fd > _max_fd)
				_max_fd = fd;
			std::cout << "Setting up " << lstn->host << ":" << lstn->port << "..." << std::endl;
		}
	} */
    if (_max_fd == 0)
		err.setAndPrint(29, "Cluster::setup");
	else
		return (0);
    return (0); // gerek olmayabilir
}

void Cluster::clean()
{
    /* for ( std::map<long, ClusterServer>::iterator it = _servers.begin(); it != _servers.end(); it++ )
        it->second.clean(); */
}
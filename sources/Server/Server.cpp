#include "../../includes/Server.hpp"

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
	(void)socket;
	return 1;
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
		fd[i] = create_server(_port, _host);
	}
}

int Server::run()
{
    fd_set read_fd_set;
    fd_set write_fd_set = {0};
    int new_fd, ret_val;
    // int check_probl = 0;
    socklen_t addrlen;
    int all_connections[MAX_CONNECTIONS];
    std::string kfe;

    for (int i = 0; i < getNbPort(); i++)
    {
        if (fd[i] == -1)
            return -1;
    }

    std::fill_n(all_connections, MAX_CONNECTIONS, -1);

    while (true)
    {
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);

        for (int i = 0; i < MAX_CONNECTIONS; i++)
        {
            if (all_connections[i] >= 0)
                FD_SET(all_connections[i], &read_fd_set);
        }

        ret_val = select(FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);

        if (ret_val >= 0)
        {
            for (int i = 0; i < getNbPort(); i++)
            {
                if (FD_ISSET(fd[i], &read_fd_set))
                {
                    new_fd = accept(fd[i], (struct sockaddr *)&new_addr, &addrlen);
                    fcntl(fd[i], F_SETFD, FD_CLOEXEC);

                    if (new_fd >= 0)
                    {
                        for (int j = 0; j < MAX_CONNECTIONS; j++)
                        {
                            if (all_connections[j] < 0)
                            {
                                all_connections[j] = new_fd;
                                break;
                            }
                        }
                    }

                    if (new_fd == -1)
                    {
                        std::cerr << "accept() failed for fd\n" << strerror(errno) << std::endl;
                    }

                    ret_val--;

                    if (!ret_val)
                        continue;
                }
            }

            for (int i = 1; i < MAX_CONNECTIONS; i++)
            {
                if ((all_connections[i] > 0) && (FD_ISSET(all_connections[i], &read_fd_set)))
                {
                    // int err = read_connection(all_connections[i]);

                    /* if (err == 0)
                    {
                        ResHandler test;
                        char buf[DATA_BUFFER + 1];
                        memset(buf, 0, sizeof(buf));
                        strcpy(buf, buffu.c_str());
                        test.MainServer.bando = buffu;
                        std::string conttype;
                        test.parse_buf(buf, test.MainServer.filename, conttype);
                        ParserRequest pr(buf);
                        test.MainServer.host = pr.getRequest().host;
                        test.MainServer.protocol = pr.getRequest().protocol;
                        test.MainServer.type = pr.getRequest().method;
                        test.MainServer.path = pr.getRequest().location;
                        test.MainServer.content_len = pr.getRequest().content_length;
                        test.MainServer.buffit = std::string(buf);
                        test.CheckModiDate();
                        test.setDate();
                        test.Erostatus();
                        test.Methodes(FileConf);
                        int checkWrit = write(all_connections[i], test.TheReposn.c_str(), (test.TheReposn.size() + 1));

                        if (checkWrit == 0)
                            check_probl = 0;
                        else if (checkWrit == -1)
                            check_probl = -1;

                        close(all_connections[i]);
                        fcntl(all_connections[i], F_SETFD, FD_CLOEXEC);
                        FD_CLR(all_connections[i], &read_fd_set);

                        buffu = "";
                        test.TheReposn = "";
                        all_connections[i] = -1;

                        break;
                    }
                    if (err == 5)
                    {
                        close(all_connections[i]);
                        all_connections[i] = -1;
                    }

                    if (err == -1)
                    {
                        break;
                    } */
                }
            }
        }
    }

    close(new_fd);
    close(ret_val);
    return 0;
}
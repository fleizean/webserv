#pragma once

#include "Include.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"
#include "Server.hpp"
#include "ServerMembers.hpp"

class Client
{
private:
    size_t                          _contentLen;
    Server*                         _sv;
    std::vector<ServerMembers*>     _confServers;
    Request                         _request;
    int                             _locationIndex;
    std::string                     _responseHeader;
    unsigned int                    _host;
    int                             _port;
    std::string                     _method;
    int                             _multi;
    int                             _status;
    int                             _isFav;
    std::string                     _body;
    std::string                     _buffer;

    Error                           err;

public:
    Client(Server* sv, std::vector<ServerMembers*> _confServers);
    Client();
    ~Client();

    std::string     process(std::string multi);

    int             getStatus() const;
    int				getMulti() const;
	int				getIsFav() const;
	std::string		getMethod() const;
	size_t			getContentLen() const;
	std::string		getBody() const;
	ServerMembers*  getServerForRequest(t_listen& address, std::vector<ServerMembers*>& servers, Request &req);

    void	        setParserRequest(std::string buffer);
	void	        setMulti(int multi);
	void	        setIsFav(int isFav);
	void	        setStatus(int status);
	void	        setMethod(std::string method);
	void	        setContentLen(int contentLen);
	void	        setBody(std::string body);

    std::string		getNullResponse();
};
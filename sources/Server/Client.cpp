#include "../../includes/Client.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(Server* sv, std::vector<ServerMembers*> confServers)
{
	_contentLen = 0;
	_sv = sv;
	_confServers = confServers;
	_locationIndex = 0;
    _responseHeader = "";
	_host = sv->getHost();
	_port = sv->getPort();
	_method = "";
	_multi = 0;
	_status = 0;
    _isFav = 0;
	_body = "";
} 

int 			Client::getStatus() const { return this->_status; }

int 			Client::getMulti() const { return this->_multi; }

int 			Client::getIsFav() const { return this->_isFav; }

std::string		Client::getMethod() const { return this->_method; }

size_t			Client::getContentLen() const { return this->_contentLen; }

std::string		Client::getBody() const { return this->_body; }

void			Client::setMulti(int multi) { this->_multi = multi; }

void			Client::setStatus(int status) { this->_status = status; }

void   			Client::setMethod(std::string method) { this->_method = method; }

void   			Client::setContentLen(int contentLen) { this->_contentLen = contentLen; }

void   			Client::setIsFav(int isFav) { this->_isFav = isFav; }

void   			Client::setBody(std::string body) { this->_body = body; }



void	        Client::setParserRequest(std::string buffer)
{
	Request pr(buffer);
	_request = pr;
	_buffer = buffer;

	setStatus(_request.getStatus());
	setMulti(_request.getMulti());
	setMethod(_request.getMethod());
	setContentLen(_request.getContentLength());
	setBody(_request.getBody());
	if (_request.getLocation() == "/favicon.ico")
	{
		_isFav = 1;
	}
}

std::string		Client::process(std::string multiBody)
{
	std::cout << BOLD_YELLOW << "Processing..." << RESET << std::endl;
	ServerMembers* 	matchedServer;

	matchedServer = getServerForRequest(_request.getListen(), _confServers, _request);
	if (matchedServer == NULL)
	{
		_responseHeader = getNullResponse();
		return (_responseHeader);
	}
	else
	{
		Response response(_request, _confServers, matchedServer, multiBody);
		response.setBando(_buffer);
		response.checkModifyDate();
		response.setDate();
		response.errorStatus();
		response.run();
		if (response.getResponseHeader() == "")
		{
			_responseHeader = getNullResponse();
			return (_responseHeader);
		}

		_responseHeader = response.getResponseHeader();
		return _responseHeader;
	}
	
}

std::string		Client::getNullResponse()
{
	std::string resHeader;
	resHeader += "HTTP/1.1 404 OK" ;
	resHeader += "\nContent-Type: text/html" ;
	resHeader += "\n\n";
	resHeader += "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n</head>\n<a href=\"https://quickabdest.com/\" target=\"_blank\"/>Maalesef isteğin başarısız bir abdest alıp tekrar dene.</html>";

	return resHeader;
}

ServerMembers*	Client::getServerForRequest(t_listen& address, std::vector<ServerMembers*>& servers, Request &req)
{
	std::vector<ServerMembers*>	possibleServers;
	address.host = 2130706433;

	for (std::vector<ServerMembers *>::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		if (req.getIsDomain() == true)
		{
			if (!(*it)->getServerName().empty())
			{
				
				for (std::vector<std::string>::const_iterator sit = (*it)->getServerName().begin(); sit != (*it)->getServerName().end(); sit++)
				{
					if (*sit == req.getHost() && address.port == (*it)->getListen().port)
					{
						return (*it);
					}
				}
			}
		}
		else
		{
			if (address.port == (*it)->getListen().port)
				possibleServers.push_back((*it));
		}
	}

	if (possibleServers.empty())
		return NULL;

	return possibleServers[0];

/* 	ServerMembers* matchingServer = nullptr;
	ServerMembers* firstServer = nullptr;

	bool first = true;

	for (std::vector<ServerMembers *>::const_iterator it = servers.begin(); it != servers.end(); it++)
    {
		if (it == servers.begin())
			firstServer = (*it);
		if (!(*it)->getServerName().empty())
		{
			for (std::vector<std::string>::const_iterator sit = (*it)->getServerName().begin(); sit != (*it)->getServerName().end(); sit++)
			{
				if (*sit == req.getHost() && address.port == (*it)->getListen().port)
				{
					address.host = 2130706433;
					return (*it);
				}
			}
			first = false;
		}
		else 
		{
 			if (address.port == (*it)->getListen().port)
        	{
        	    matchingServer = (*it);
        	}
		}
    }
	if (first == false || matchingServer != nullptr)
    {
        matchingServer = firstServer;
    }

    return matchingServer; */
}


#pragma once

#include "Include.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Listen.hpp"

class Response
{
private:
	std::string					_response;
	std::string					_path;
	int							_code;
	std::string					_type;
	bool						_isAutoIndex;
	t_listen					_hostPort;
	std::map<int, std::string>	_errorMap;

public:
	Response();
	Response(const Response &rhs);
	~Response();

	Response &operator=(const Response &rhs);

	// Get Area
	std::string getResponse();
	int			getStatusCode();

	void	setAllow(std::vector<std::string> methods);
	void	setAllow(const std::string& allow = "");
	void	setContentLanguage(const std::string& lang);
	void	setContentLength(size_t size);
	void	setContentLocation(const std::string&path, int code);
	void	setContentType(std::string type, std::string path);
	void	setDate();
	void	setLastModified(const std::string& path);
	void	setLocation(int code, const std::string& redirect);
	void	setRetryAfter(int code, int sec);
	void	setServer();
	void	setTransferEncoding();
	void	setWwwAuthenticate(int code);
	void	setValues(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang);

	void	resetValues();

	//void    createResponse(Request *request, Server *server, Location *location);
    //std::string notAllowed(std::vector<std::string> methods, const std::string& path, int code, const std::string& lang);
    //std::string	writeHeader(void);
    //std::string	readHtml(const std::string& path);
    //int			readContent(Server *server);
    //std::string		getHeader(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang);
    //std::string		getStatusMessage(int code);
    //std::string     getPage(const char *path, std::string const &host, int port);
    //std::string     getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port);

    //void    GETmethod(Request* request, Server* scope, Location* location);
    //void	DELETEmethod(Request* request, Server* scope, Location* location);
    //void	POSTmethod(Request* request, Server* scope, Location* location);
};
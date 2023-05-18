#pragma once

#include <sys/time.h>
#include <sys/stat.h>
#include <vector>
#include "Server.hpp"
#include "Location.hpp"
#include "Listen.hpp"
#include "Request.hpp"


class Response
{
private:
	std::string                 _response;
    int                          statusCode;
    bool                        _isAutoIndex;
    std::string					_type;
    std::map<int, std::string>	_errorMap;

    std::vector<std::string>	_index;//bunu araştır???
    std::string					_path;// local path for request
	std::string					_cgi_pass;


    std::string					_allow;
	std::string					_contentLanguage;
	std::string					_contentLength;
	std::string					_contentLocation;// public part of the path
	std::string					_contentType;
	std::string					_date;
	std::string					_lastModified;
	std::string					_location;
	std::string					_retryAfter;
	std::string					_server;
	std::string					_transferEncoding;
	std::string                 _wwwAuthenticate;

public:
	Response();
	~Response();

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

	void    createResponse(Request *request, Server *server, Location *location);
    std::string notAllowed(std::vector<std::string> methods, const std::string& path, int code, const std::string& lang);
    std::string	writeHeader(void);
    std::string	readHtml(const std::string& path);
    int			readContent(Server *server);
    std::string		getHeader(size_t size, const std::string& path, int code, std::string type, const std::string& contentLocation, const std::string& lang);
    std::string		getStatusMessage(int code);
    std::string     getPage(const char *path, std::string const &host, int port);
    std::string     getLink(std::string const &dirEntry, std::string const &dirName, std::string const &host, int port);

    void    GETmethod(Request* request, Server* server);
    void	DELETEmethod();
    void	POSTmethod(Request* request, Server* server);
};
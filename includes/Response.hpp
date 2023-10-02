#pragma once

#include "Include.hpp"
#include "Request.hpp"
#include "ServerMembers.hpp"
#include "Location.hpp"
#include "Cgi.hpp"
#include <sys/stat.h>
#include <sys/time.h>

class Response
{
private:
	std::string							_path;
	std::string							_protocol;
	std::string							_requestHeader;
	std::string							_type;
	std::string							_contentType;
	std::string							_time;
	std::string							_http;
	std::string							_host;
	std::string							_encoding;
	std::string							_fileName;
	std::string							_cgiType;
	std::string							_cgiPath;

	int									_code;
	int									_contentLen;
	int									_maxBody;
	std::string							_uriRoot;
	
	std::string							_body;
	std::vector<std::string>			_postValues;
	std::vector<std::string> 			_allowedMethods;

	unsigned int						_redirectionType;
	std::string							_redirectionURI;

	bool								_hasRedirection;
	bool 								_isUpload;
	bool								_postMethod;
	bool								_redirectionLocation;


	std::map<std::string, std::string>	mp;
	std::map<std::string, std::string>	cgi;

	std::string							_errorRep[520];
	std::string 						_responseHeader;

	Request								_req;
	std::vector<ServerMembers*> 		_servers;
	ServerMembers*						_matchedServer;
	std::string							_multiBody;
	
public:
	Response();
	Response(Request req, std::vector<ServerMembers*> servers, ServerMembers* matchedServer, std::string multiBody);
	~Response();
	void	parseQueryString(const std::string &queryString);
	int		postMethodes();
	void	run();
	void	resetHTML();
	void	errorStatus();
	int		getMethodes();
	void	modifyResponseHeader();
	void 	deleteMethodes();
	void	handleDeleteRequest();
	int		fileExist(const char *fileName);
	void	setDate();
	void	getContentType(std::string path);
	void	errorPage();
	void	setupRequest();
	bool	checkIfPathIsFile(const char * path);
	void	setBuffer(std::string requestHeader);
	void	processRequest();
	bool 	checkCgiForConfig();

	std::string getResponseHeader();
	std::string fAutoIndex(const char *path);
	std::string createDirectoryLink(std::string const &dirEntry, std::string directory, std::string const &host);
	
};
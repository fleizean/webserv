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
	std::string							_bando;
	std::string							_statusCode;
	std::string							_type;
	std::string							_contentType;
	std::string							_time;
	std::string							_modifyTime;
	std::string							_http;
	std::string							_server;
	std::string							_host;
	std::string							_encoding;
	std::string							_fileName;
	std::string							_buf;
	std::string							_cgiType;
	std::string							_buffit;
	std::string							_serverName;
	std::string							_upload;
	std::string							_cgiPath;

	char*								_files;
	int									_code;
	int									_contentLen;
	int									_port;
	int									_maxBody;
	std::string							_contentLength;
	std::string							_uriRoot;
	std::string							_configName;
	std::string							_index;
	
	std::string							_body;
	std::vector<std::string>			_postValues;
	std::vector<std::string> 			_allowedMethods;

	unsigned int						_redirectionType;
	std::string							_redirectionURI;

	bool								_hasRedirection;
	bool 								_isUpload;
	bool								_postmethod;


	std::map<std::string, std::string>	mp;
	std::map<std::string, std::string>	cgi;

	std::string							_errorRep[520];
	std::string 						_responseHeader;

	Request								_req;
	std::vector<ServerMembers*> 		_servers;
	ServerMembers*						_matchedServer;
	std::string							_multiBody;
	/* unused */
	
public:
	Response();
	std::string getResponseHeader();
	Response(Request req, std::vector<ServerMembers*> servers, ServerMembers* matchedServer, std::string multiBody);
	~Response();
	void	parseQueryString(const std::string &query_string);
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
	void	checkModifyDate();
	void	parse_buf(char *buf, std::string& filename, std::string& content_type);
	void	getContentType(std::string path);
	void	errorPage();
	void	setupRequest();
	bool	checkIfPathIsFile(const char * path);
	void	setBando(std::string bando);
	void	processRequest();
	bool 	checkCgiForConfig();


	std::string fAutoIndex(const char *path);
	std::string createDirectoryLink(std::string const &dirEntry, std::string Directory, std::string const &host);
	std::string uploadFile(std::string sear, std::string buffer);
	
	/* printResponse details */
	void	printResponse();
};
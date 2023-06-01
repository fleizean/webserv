#pragma once

#include "Include.hpp"
#include "Request.hpp"
#include "ServerMembers.hpp"
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
	int									_contentLen;
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
	std::string							_postName[10000];
	std::string							_postValue[10000];
	char*								_files;
	int									_code;
	int									_port;
	int									_envj;
	int									_maxBody;
	std::string							_contentLength;
	std::string							_uriRoot;
	std::string							_configName;
	std::string							_index;
	
	std::map<std::string, std::string>	mp;

	std::string							_errorRep[520];
	std::string 						_responseHeader;

	Request								_req;
	std::vector<ServerMembers*> 		_servers;
	char**								_envp;
	/* unused */
	Response();
public:
	Response(Request req, std::vector<ServerMembers*> servers, char** envp);
	~Response();
	void	parseQueryString(const std::string &query_string);
	int		postMethodes();
	void	methodes(std::string FileConf);
	void	resetHTML();
	void	errorStatus();
	int		getMethodes();
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

	std::string fAutoIndex(const char *path);
	std::string createDirectoryLink(std::string const &dirEntry, std::string Directory, std::string const &host);
	std::string uploadFile(std::string sear, std::string buffer);

	/* printResponse details */
	void	printResponse();
};
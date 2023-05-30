#pragma once

#include "Include.hpp"

class CGI
{
private:
		std::string	_body;
		std::string	_cgi;
		std::string	_type;
		std::string	_method;
		std::string	_file;
		std::string	_query;
		std::string	_host;
		std::string _contentType;
		std::string _contentLength;
		std::string _pathTranslated;
		std::string _boundary;
		int _port;
		std::map<std::string, std::string>	_env;
		char    **_createEnv(char **envp, std::string pathInfo) const;
public:
    CGI();
    CGI(const CGI &src);
	CGI &operator=(const CGI &src);
    ~CGI(){};
	std::string	execute(const std::string& scriptName, char **envp);
	void	setBody(std::string body);
	void	setScript(std::string cgi);
	void	setMethod(std::string method);
	void	setPath(std::string file);
	void	setType(std::string type);
	void	setQuery(std::string query);
	void	setAddr(std::string host);
	void	setPort(int	port);
	void    setContentType(std::string contentType);
	void    setContentLength(std::string contentLength);
	void    setPathTranslated(std::string pathTranslated);
	void    setBoundary(std::string boundary);
};
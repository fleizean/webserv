#pragma once

#include "Include.hpp"
#include <unistd.h>
#include <sstream>
#include "Location.hpp"
#include "ServerMembers.hpp"
#include <sys/stat.h>
#include <sys/time.h>
#include "Cgi.hpp"


class Response
{
private:
    
public:
    std::string	path;
    std::string	protocol;
    std::string	bando;
    std::string	StatusCode;
    std::string	type;
    std::string	content_type;
    int	content_len;
    std::string	time;
    std::string	Modiftime;
    std::string	http;
    std::string	server;
    std::string	host;
    std::string	encoding;
    std::string	filename;
    std::string	buf;
    std::string	cgitype;
    std::string	buffit;
    std::string	serv_name;
    std::string	postname[10000];
    std::string	postvalue[10000];
    char*		files;
    int			code;
    std::map<std::string, std::string> mp;
    int			port;
    int			envj;
    int			max_body;
    std::string	content_length;
    std::string	uril_root;
    std::string	ConfigName;
    std::string	index;

    std::string	TheReposn;
	std::string	EroRep[520];

    std::vector<ServerMembers*> srvr;
    Cgi g;
    Response();
    ~Response();

    void	ParseQueryString_(const std::string &query_string);
    int     POSTMethodes();
    void	Methodes(std::vector<ServerMembers*> server);

    void	    reseat(void);
    void	    Erostatus(void);
    int		    GetMethodes(void);
    void 	    DeleteMethodes(void);
    void	    DeletePathHandler(void);
    int		    filexist(const char *fileName);
    void	    setDate(void);
    void	    CheckModiDate(void);
    void	    parse_buf(char *buf, std::string& filename, std::string& content_type);
    void	    GetContent_Type(std::string path);
    void	    ErrorPage(void);
    bool        CheckifPathisFile(const char * path);
    std::string FautoIndex(const char *path);
    std::string geit(std::string const &dirEntry, std::string Directory, std::string const &host);
};
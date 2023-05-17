#pragma once

#include <string>
#include <map>
#include <list>
#include <vector>
#include "Error.hpp"

class Request
{
private:
	std::string									_method;
	std::string									_version;
	std::map<std::string, std::string>			_headers;
	std::map<std::string, std::string>			_env_for_cgi;
	int											_ret;
	std::string									_body;
	int											_port;
	std::string									_path;
	std::string									_query;
	std::list<std::pair<std::string, float> >	_lang;
	const std::string&							_raw;

	/* Avaible http methods */
	static	std::vector<std::string>	methods;

	/* ------> Parsing <------ */
	int			readFirstLine(const std::string& line);
	int			readPath(const std::string& line, size_t i);
	int			readVersion(const std::string& line, size_t i);
	int			checkMethod();
	int			checkPort();
	void		findQuery();
	std::string formatHeaderForCGI(std::string& key);
	std::string	nextLine(const std::string &str, size_t& i);
	void		setLang();
	/* ----------------------- */

	Request();
	Request(const Request &rhs);

public:
	Request(const std::string& str);
	~Request();
	Request& operator=(const Request &rhs);

	const std::map<std::string, std::string>&			getHeaders() const;
	const std::map<std::string, std::string>&			getEnv() const;
	const std::string&									getMethod() const;
	const std::string&									getVersion() const;
	int													getRet() const;
	const std::string&									getBody() const;
	int													getPort() const;
	const std::string&									getPath() const;
	const std::string&									getQuery() const;
	const std::string&									getRaw() const;
	const std::list<std::pair<std::string, float> >&	getLang() const;

	/*** SETTERS **/
	void	setBody(const std::string& line);
	void	setRet(int);
	void	setMethod(const std::string &method);

	/*** UTILS ****/
	int		parse(const std::string& str);
	void	resetHeaders();
	void	stripAll();
	void	displayHeaders() const;
	
	static std::vector<std::string>		initMethods();
};
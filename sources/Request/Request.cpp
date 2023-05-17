#include "../../includes/Request.hpp"

std::vector<std::string>		Request::initMethods()
{
	std::vector<std::string>	methods;

	methods.push_back("GET");
	methods.push_back("HEAD");
	methods.push_back("POST");
	methods.push_back("PUT");
	methods.push_back("DELETE");
	methods.push_back("OPTIONS");
	methods.push_back("TRACE" );

	return methods;
}

std::vector<std::string>	Request::methods = Request::initMethods();

Request::Request(const std::string& str) : _method(""), _version(""), _ret(200), _body(""), _port(80), _path(""), _query(""), _raw(str)
{
    Error err(0);
    this->resetHeaders();
    this->_env_for_cgi.clear();
    this->parse(str);
    if (this->_ret != 200)
        err.setAndPrint(35, std::to_string(this->_ret));
    
}

Request::~Request() {}

Request& Request::operator=(const Request& rhs)
{
    this->_headers = rhs.getHeaders();
	this->_env_for_cgi = rhs.getEnv();
	this->_method = rhs.getMethod();
	this->_version = rhs.getVersion();
	this->_ret = rhs.getRet();
	this->_body = rhs.getBody();
	this->_port = rhs.getPort();
	this->_path = rhs.getPath();

	return *this;
}

/* ---------------> GETTER FUNC <--------------- */

const std::map<std::string, std::string>&	Request::getHeaders() const
{
	return this->_headers;
}

const std::map<std::string, std::string>&	Request::getEnv() const
{
	return this->_env_for_cgi;
}

const std::string&	Request::getMethod() const
{
	return this->_method;
}

const std::string&	Request::getVersion() const
{
	return this->_version;
}

int					Request::getRet() const
{
	return this->_ret;
}

const std::string&	Request::getBody() const
{
	return this->_body;
}

int					Request::getPort() const
{
	return this->_port;
}

const std::string&			Request::getPath() const
{
	return this->_path;
}

const std::string&			Request::getQuery() const
{
	return this->_query;
}

const std::string&			Request::getRaw() const
{
	return this->_raw;
}

const std::list<std::pair<std::string, float> >&	Request::getLang() const
{
	return this->_lang;
}

/* --------------------------------------------- */

/* ---------------> SET FUNC <--------------- */

void	Request::setBody(const std::string& str)
{
	char	strip[] = {'\n', '\r'};

	this->_body.assign(str);
	for (int i = 0; i < 4; i++)
		if (this->_body.size() > 0 && this->_body[this->_body.size() - 1] == strip[i % 2])
			pop(this->_body);
		else
			break ;
}

void	Request::setRet(int ret)
{
	this->_ret = ret;
}

void	Request::setMethod(const std::string &method)
{
	this->_method = method;
}

void	Request::setRet(int ret)
{
	this->_ret = ret;
}

void	Request::setMethod(const std::string &method)
{
	this->_method = method;
}

/* ------------------------------------------ */

/* utils */
static std::string&					pop(std::string& str)
{
	if (str.size())
		str.resize(str.size() - 1);
	return str;
}
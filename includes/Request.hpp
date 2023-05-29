#pragma once

#include "Include.hpp"
#include "Listen.hpp"

class Request
{
private:
    std::string	_method;
    std::string	_location;
    std::string	_protocol;
    std::string	_host;
    int	        _port;
    size_t		_content_length;
    std::string _accept_language;
    std::string _connection;
    t_listen    _listen;

    std::string		m_request;
public:
    Request(const char *buffer);
    ~Request();

    const std::string&		getRequestStr(void) const;

    void	parse(void);
	void	parseLine(std::string& line);

    void	addMethod(std::stringstream& ss, std::string& word);
	void	addHost(std::stringstream& ss);
	void	addContentLength(std::stringstream& ss);
    void    addAcceptLanguage(std::stringstream& ss);
    bool    checkPort();
    void    addConnection(std::stringstream& ss);

    /* Getters */
    std::string const &getMethod() const;
    std::string const &getLocation() const;
    std::string const &getProtocol() const;
    std::string const &getHost() const;
    std::string const &getAcceptLanguage() const;
    size_t const &getContentLength() const;
    int	const &getPort() const;
    std::string const &getConnection() const;
    t_listen &getListen();
    
    /* Print All */
	void printAll();
};
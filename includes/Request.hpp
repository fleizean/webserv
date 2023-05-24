#pragma once

#include "Include.hpp"

class Request
{
private:
    std::string	_method;
    std::string	_location;
    std::string	_protocol;
    std::string	_host;
    size_t		_content_length;

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

    /* Getters */
    std::string const &getMethod() const;
    std::string const &getLocation() const;
    std::string const &getProtocol() const;
    std::string const &getHost() const;
    size_t const &getContentLength() const;

    /* Print All */
	void printAll();
};
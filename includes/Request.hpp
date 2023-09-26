#pragma once

#include "Include.hpp"
#include "Listen.hpp"
#include "Utils.hpp"


class Request
{
private:
    std::string	    _method;
    std::string	    _location;
    std::string	    _protocol;
    std::string	    _host;
    std::string     _fullHost;
    std::string     _accept_language;
    std::string     _connection;
    std::string     _contentType;
    std::string     _fileName;
    int             _multi;
    std::string     _multiPart;
    std::string     _body;
    int             _status;
    int	            _port;
    size_t		    _content_length;
    t_listen        _listen;
    bool            _isDomain;

    std::string		m_request;

    std::string     _firstMediaType;
public:
    Request();
    Request(std::string buffer);
    ~Request();

    const std::string&		getRequestStr(void) const;

    void    parseBody();
    void	parse(void);
	void	parseLine(std::string& line);
    void    parseMultiPart(); 

    void	addMethod(std::stringstream& ss, std::string& word);
	void	addHost(std::stringstream& ss);
	void	addContentLength(std::stringstream& ss);
    void    addAcceptLanguage(std::stringstream& ss);
    bool    checkPort();
    void    addConnection(std::stringstream& ss);
    void    addContentType(std::stringstream& ss);
    void    addFileName(std::stringstream& ss);
    void    addAcceptControlMethod(std::stringstream& ss);
    bool    isDomain(const std::string& host);

    /* Getters */
    std::string const   &getFirstMediaType() const;
    std::string const   &getMethod() const;
    std::string const   &getLocation() const;
    std::string const   &getProtocol() const;
    int         const   &getStatus() const;
    std::string const   &getMultiPart() const;
    std::string const   &getHost() const;
    std::string const   &getAcceptLanguage() const;
    std::string const   &getConnection() const;
    std::string const   &getContentType() const;
    std::string const   &getFileName() const;
    std::string const   &getBody() const;
    bool const          &getIsDomain() const;
    t_listen            &getListen();
    size_t const        &getContentLength() const;
    int	const           &getPort() const;
    int const           &getMulti() const;
    std::string const   &getFullHost();
    void                clear();

    void                addFirstMediaType(std::stringstream& acceptHeader);
    /* Print All */
	void                printAll();
};
#include "../../includes/Request.hpp"

/* utils */

bool Request::checkPort()
{
    string host_temp = "127.0.0.1";
    size_t i = _host.find_first_of(':');

    if (i == std::string::npos)
        this->_port = 80;
    else
    {
        host_temp = _host.substr(0, i);
        std::string tmp(_host.substr(i + 1, _host.size() - (i + 1)));
        this->_port = std::stoi(tmp.c_str());
        if (_port == 0)
            return false;
        _host = host_temp;
        _listen.host = strToIp(_host);
        _listen.port = _port;
    }
    return true;
}

/* ----- */

void Request::clear(){
    _method = "";
    _location = "";
    _protocol = "";
    _host = "";
    _accept_language = "";
    _connection = "";
    _contentType = "";
    _fileName = "";
    _body = "";
    _port = 0;
    _content_length = 0;
    _listen.host = 0;
    _listen.port = 0;
}

Request::Request() {}

void Request::parseBody()
{
    this->_body = this->m_request.substr(this->m_request.find("\r\n\r\n") + 4);
}

Request::Request(const char *buffer)
{
    m_request = buffer;
    /* std::cout << BOLD_RED << "Buffer socket: " << RESET << std::endl;
    std::cout << buffer << std::endl;  */
    parseBody();
    clear();
    parse();
    printAll();
}

Request::~Request() {}

void Request::parse()
{
    std::stringstream   ss(m_request);
    std::string         line;
    
    while (std::getline(ss, line))
        parseLine(line);
}

void    Request::parseLine(std::string& line)
{
    std::stringstream   ss(line);
    std::string         word;

    ss >> word;
    if (isValidMethod(word))
        addMethod(ss, word);
    else if (word == "Host:")
        addHost(ss);
    else if (word == "Content-Length:")
        addContentLength(ss);
    else if (word == "Accept-Language:")
        addAcceptLanguage(ss);
    else if (word == "Connection:")
        addConnection(ss);
    else if (word == "Content-Type:")
        addContentType(ss);
    else if (word == "Content-Disposition:")
        addFileName(ss);
    else if (word == "Accept:")
        addFirstMediaType(ss);
    else if (word == "Access-Control-Request-Method:")
        addAcceptControlMethod(ss);
}

void Request::addAcceptControlMethod(std::stringstream& ss)
{
    std::string word;

    ss >> word;
    if (word == "DELETE")
        _method = word;
}

void Request::addFirstMediaType(std::stringstream& ss)
{
    std::string mediaType;

    std::getline(ss, mediaType, ',');
    // Gelen medya türünden boşlukları temizle
    mediaType.erase(std::remove_if(mediaType.begin(), mediaType.end(), ::isspace), mediaType.end());
    _firstMediaType = mediaType;
}

void    Request::addFileName(std::stringstream& ss)
{
    std::string word;

    ss >> word;
    ss >> word;
    ss >> word;
    size_t pos = 0;
    if (pos != std::string::npos && pos + 9 < word.size())
    {
        _fileName = word.substr(9);
    }
}

void    Request::addContentType(std::stringstream& ss)
{
    std::string word;
    ss >> word;
    _contentType = word;
}

void    Request::addConnection(std::stringstream& ss)
{
    std::string word;

    ss >> word;
    _connection = word;
}

void    Request::addMethod(std::stringstream& ss, std::string& word)
{
    Error err(0);       
    _method = word;
    if (!(ss >> _location))
        err.setAndPrint(45, "Request::addMethod");
    if (!(ss >> _protocol))
        err.setAndPrint(46, "Request::addMethod");
    if (_protocol != "HTTP/1.1")
        err.setAndPrint(44, "Request::addMethod");
}

void    Request::addHost(std::stringstream& ss)
{
    Error err(0);
    if (!(ss >> _host))
        err.setAndPrint(47, "Request::addHost");
    _fullHost = _host;
    if (!checkPort())
        err.setAndPrint(48, "Request::addHost");
}

void    Request::addContentLength(std::stringstream& ss)
{
    std::string word;

    ss >> word;
    _content_length = std::stoi(word);
}

void    Request::addAcceptLanguage(std::stringstream& ss)
{
    Error err(0);
    std::string word;

    if (!(ss >> word))
        err.setAndPrint(49, "Request::addAcceptLanguage");
    size_t i = word.find_first_of('-');
    _accept_language = word.substr(0, i);
}

/* Getters */
std::string const &Request::getMethod() const { return this->_method; }
std::string const &Request::getLocation() const { return this->_location; }
std::string const &Request::getProtocol() const { return this->_protocol; }
std::string const &Request::getHost() const { return this->_host; }
std::string const &Request::getAcceptLanguage() const { return this->_accept_language; }
const std::string& Request::getRequestStr() const { return (m_request); }
std::string const &Request::getContentType() const { return this->_contentType; }
std::string const &Request::getConnection() const { return this->_connection; }
std::string const &Request::getFileName() const { return this->_fileName; }
std::string const &Request::getBody() const { return this->_body; }

size_t const &Request::getContentLength() const { return this->_content_length; }
int const &Request::getPort() const { return this->_port; }
t_listen &Request::getListen() { return _listen; }
std::string const &Request::getFullHost() { return _fullHost; }
std::string const &Request::getFirstMediaType() const { return _firstMediaType; }

/* Other */
void Request::printAll()
{
    std::cout << BLUE << "----------> Request Parse Area <----------" << RESET << "\n";
    std::cout << BOLD_MAGENTA << "Http-Method: " << RESET << this->getMethod() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Location: " << RESET << this->getLocation() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Protocol: " << RESET << this->getProtocol() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Host: " << RESET << this->getHost() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Port: " << RESET << this->getPort() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Content-Length: " << RESET << this->getContentLength() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Accept-Language: " << RESET << this->getAcceptLanguage() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Connection: " << RESET << this->getConnection() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Content-Type: " << RESET << this->getContentType() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Filename: " << RESET << this->getFileName() << "\n";
    std::cout << BOLD_MAGENTA << "Http-Accept-Media-Type: " << RESET << this->getFirstMediaType() << "\n";
}
#include "../../includes/Request.hpp"

Request::Request(const char *buffer)
{
    m_request = buffer;
    parse();
    printAll();
}

Request::~Request() {}

const std::string&	Request::getRequestStr() const { return (m_request); }

void Request::parse()
{
    std::stringstream	ss(m_request);
	std::string			line;
    
    while (std::getline(ss, line))
		parseLine(line);
}

void	Request::parseLine(std::string& line)
{
	std::stringstream	ss(line);
	std::string			word;

	ss >> word;
	if (isValidMethod(word))
		addMethod(ss, word);
	else if (word == "Host:")
		addHost(ss);
	else if (word == "Content-Length:")
		addContentLength(ss);
}


void	Request::addMethod(std::stringstream& ss, std::string& word)
{
	_method = word;
	ss >> _location;
	ss >> _protocol;
}

void	Request::addHost(std::stringstream& ss)
{
	ss >> _host;
}

void	Request::addContentLength(std::stringstream& ss)
{
	std::string	word;

	ss >> word;
	_content_length = std::stoi(word);
}

/* Getters */
std::string const &Request::getMethod() const { return this->_method; }
std::string const &Request::getLocation() const { return this->_location; }
std::string const &Request::getProtocol() const { return this->_protocol; }
std::string const &Request::getHost() const { return this->_host; }
size_t const &Request::getContentLength() const { return this->_content_length; }

/* Other */
void Request::printAll()
{
    std::cout << BLUE << "----------> Request Parse Area <----------" << RESET << "\n";
    std::cout << RED << "method: " << this->getMethod() << "\n";
    std::cout << RED << "location: " << this->getLocation() << "\n";
    std::cout << RED << "protocol: " << this->getProtocol() << "\n";
    std::cout << RED << "host: " << this->getHost() << "\n";
    std::cout << RED << "content_length: " << this->getContentLength() << "\n" << RESET;
}
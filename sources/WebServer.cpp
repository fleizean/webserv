#include "../includes/WebServer.hpp"

WebServer::WebServer() : _configContent(), serverBlock(false), mainBlock(true), locationBlock(false) {}

WebServer::WebServer(WebServer const &rhs) : _configContent(rhs._configContent), serverBlock(rhs.serverBlock), mainBlock(rhs.mainBlock), locationBlock(rhs.locationBlock) {}

WebServer::~WebServer() {}

WebServer &WebServer::operator=(WebServer const &rhs)
{
	_configContent	= rhs._configContent;
	serverBlock		= rhs.serverBlock;
	mainBlock		= rhs.mainBlock;
	locationBlock	= rhs.locationBlock;
	return *this;
}

std::vector<Server*> WebServer::getConfig() { return this->_parsedServers; }

void WebServer::FileChecker(const string &conf_path)
{
	Error err(0);
	string contentsConfig;
	std::ifstream conf(conf_path);
	if(!conf)
		err.setAndPrint(1, "NULL");
	conf.close();
	if (!conf_path.length())
		err.setAndPrint(5, "NULL");
	if (conf_path.size() < 6 || conf_path.substr(conf_path.size() - 5) != ".conf")
		err.setAndPrint(4, "NULL");
	if (fileToString(conf_path, contentsConfig) != -1)
		contentsConfig = removeComments(contentsConfig);
	else
		err.setAndPrint(2, "NULL");
	if(!isBracketBalanced(contentsConfig))
		err.setAndPrint(7, "NULL");
	this->_configContent = contentsConfig;
	parse_server();
}

/* <---------------> Before Parsing Area <---------------> */

void WebServer::split_server(std::string configContent)
{
	Error err(0);
	std::string str = "";
	//int m_curr_line = 0;
	std::string     line;
	std::istringstream istreamfs(configContent);

	while (std::getline(istreamfs, line))
	{
		//std::cout << "mcurrent: " << m_curr_line << ". " << line << " \nmxtx : " << this->mainBlock << " " << this->serverBlock << " " << this->locationBlock << std::endl;
		//m_curr_line++;
		line = trim(line, " \t");
		if(line == "")
			continue;
		else if (line == "}")
			endScopeConf();
		else if (this->mainBlock == true)
			parseMainArea(line);
		else if (this->serverBlock == true)
			parseServerArea(line);
		else if (this->locationBlock == true)
			parseLocationArea(line);
	}
	if (this->mainBlock != true)
		err.setAndPrint(26, "split server");
	if (_parsedServers.empty())
		err.setAndPrint(27, "split server");
}

void WebServer::endScopeConf()
{
	if (this->serverBlock == true)
	{
		this->mainBlock = true;
		this->locationBlock = false;
	}
	else if (this->locationBlock == true){
		this->serverBlock = true;
		this->locationBlock = false;
	}
}


void WebServer::parseMainArea(std::string& line)
{
	// Server new_server;
	Error err(0);
	if (line != "server {")
		err.setAndPrint(8, "NULL");
	_parsedServers.push_back(new Server());
	this->serverBlock = true;
	this->mainBlock = false;
}

void WebServer::parseServerArea(std::string& line)
{
	Error err(0);
	if (line.back() != ';' && line.substr(0, 8) != "location")
		err.setAndPrint(9, "NULL");
	if (line.back() == ';')
		line.pop_back();

	std::string			word;
	std::stringstream	ss(line);
	Server *srvr =		_parsedServers.back();
	
	ss >> word;
	if (word == "listen")
		parseListen(ss, *srvr);
	else if (word == "server_name")
		parseServerName(ss, *srvr);
	else if (word == "cgi")
		parseCgi(ss, srvr->getConfigMembers());
	else if (word == "root")
		parseRoot(ss, srvr->getConfigMembers());
	else if (word == "index")
		parseIndex(ss, srvr->getConfigMembers());
	else if (word == "autoindex")
		parseAutoIndex(ss, srvr->getConfigMembers());
	else if (word == "error_page")
		parseErrorPage(ss, srvr->getConfigMembers());
	else if (word == "max_client_body_size")
		parseMaxClientBodySize(ss, srvr->getConfigMembers());
	else if (word == "location")
		parseLocation(ss, *srvr);
}

void WebServer::parseLocationArea(std::string& line)
{
	Error err(0);
	if (line.back() != ';')
	{
		err.setAndPrint(10, "Location area");
	}
	line.pop_back();

	std::string			word;
	std::stringstream	ss(line);
	Location* lctn = _parsedServers.back()->getLocations().back();
	if(!this->_tmpLocationUri.empty())
		lctn->setUri(this->_tmpLocationUri);
	ss >> word;
	if (word == "root")
		parseRoot(ss, lctn->getConfigMembers());
	else if (word == "allow")
		parseAllowedMethods(ss, *lctn);
	else if (word == "return")
		parseReturn(ss, *lctn);
	else if (word == "error_page")
		parseErrorPage(ss, lctn->getConfigMembers());
	else if (word == "autoindex")
		parseAutoIndex(ss, lctn->getConfigMembers());
	else if (word == "max_client_body_size")
		parseMaxClientBodySize(ss, lctn->getConfigMembers());
	else if (word == "index")
		parseIndex(ss, lctn->getConfigMembers());
	else
		err.setAndPrint(19, "Parse Location");
}

void WebServer::parse_server()
{
	split_server(this->_configContent);
	
}

/* <-----------------------------------------------------> */



/* <---------------> Parsing Area <---------------> */

void WebServer::parseListen(std::stringstream& ss, Server &srvr) // bitti
{
	Error err(0);
	std::string		word;
	size_t          founded_indx;

	if (!(ss >> word))
		err.setAndPrint(11, "parseListen");
	founded_indx = word.find(":");
	if (founded_indx != std::string::npos)
	{
		srvr.setHost(word.substr(0, founded_indx));
		word = word.substr(founded_indx + 1, word.size());
	}
	try
	{
		srvr.setPort(std::stoi(word));

	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "parseListen");
	}
	
}

void WebServer::parseServerName(std::stringstream& ss, Server &srvr) // bitti
{
	Error err(0);
	std::string word;

	while (ss >> word) // birden fazla gelmesi gereksiz olabilir örn: localhost deneme.com...
	{
		srvr.setServerName(word); // 2130706433 localhost unsigned int değeri atama yapmak için kullanmak zorundayız
	}
	if(srvr.getServerName().empty())
		err.setAndPrint(12, "parseServerName");
}

void WebServer::parseCgi(std::stringstream& ss, ConfigMembers &cm) // bitti gibi
{
	Error err(0);
	std::string ext;
	std::string file;

	if (!(ss >> ext) || !(ss >> file))
		err.setAndPrint(18, "parseCgi");
	cm.getCgis().insert(std::make_pair(ext, file));
	if (ss >> ext)
		err.setAndPrint(19, "parseCgi");
}

void WebServer::parseRoot(std::stringstream& ss, ConfigMembers &cm) // bitti
{
	Error err(0);
	std::string word;
	
	if (!(ss >> word))
		err.setAndPrint(13, "Root");
	
	cm.setRoot(word);
	if (ss >> word)
		err.setAndPrint(14, "Root");
}

void WebServer::parseIndex(std::stringstream& ss, ConfigMembers& cm) // bitti
{
	Error err(0);
	std::string word;

	while(ss >> word){ // 1 tane olması gerek yine üstteki gibi birden fazla saçma oluyor olabilir
		cm.getIndex().push_back(word);
	}
	if(cm.getIndex().empty())
		err.setAndPrint(16, "parseIndex");
}

void WebServer::parseMaxClientBodySize(std::stringstream& ss, ConfigMembers& cm)
{
	Error err(0);
	std::string word;

	if (!(ss >> word))
		err.setAndPrint(20, "parseMaxClientBodySize");

	try{
		cm.setMaxClientBodySize(std::stoi(word));
	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "parseMaxClientBodySize");    
	}
}

void WebServer::parseAutoIndex(std::stringstream& ss, ConfigMembers& cm)
{
	Error err(0);
	std::string		word;

	if(!(ss >> word))
		err.setAndPrint(28, "parseAutoIndex");
	if (word == "on")
		cm.setAutoIndex(true);
	else if (word == "off")
		cm.setAutoIndex(false);
	else
		err.setAndPrint(28, "parseAutoIndex");
	
	if (ss >> word)
		err.setAndPrint(28, "parseAutoIndex");
}


void WebServer::parseErrorPage(std::stringstream& ss, ConfigMembers& cm) // bakılıyor
{
	Error err(0);
	std::string word;
	size_t		error;

	if (!(ss >> word))
		err.setAndPrint(17, "parseErrorPage");
	try
	{
		error = std::stoi(word);
	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "parseErrorPage");;
	}
	if (!(ss >> word))
		err.setAndPrint(22, "parseErrorPage");
	cm.getErrorPage().insert(std::make_pair(error, word));
	if (ss >> word)
		err.setAndPrint(19, "parseErrorPage"); 
}


void WebServer::parseLocation(std::stringstream& ss, Server &srvr)// dinleyeceğimiz uri geliyor
{
	Error err(0);
	std::string		word;

	(void)srvr;
	if (!(ss >> word))
		err.setAndPrint(26, "parseLocation");
	if (word != "{")
	{
		this->_tmpLocationUri = word;
		ss >> word;
		if (word != "{")
			err.setAndPrint(26, "parseLocation");
	}
	if (ss >> word)
		err.setAndPrint(19, "parseLocation");
	_parsedServers.back()->getLocations().push_back(new Location);
	this->locationBlock = true;
	this->mainBlock = false;
	this->serverBlock = false;
}

/* <---------------> Parse Location { } Area <---------------> */

void WebServer::parseAllowedMethods(std::stringstream& ss, Location &lctn)
{
	Error err(0);
	std::string word;

	while(ss >> word)
	{
		if (!isValidMethod(word))
			err.setAndPrint(24, "null");
		lctn.getAllowedMethods().push_back(word);

	}
	if(lctn.getAllowedMethods().empty())
		err.setAndPrint(25, "Allowed Methods");
}


void WebServer::parseReturn(std::stringstream& ss, Location &lctn)
{
	Error err(0);
	std::string word;
	size_t		returnVal;

	if (!(ss >> word))
		err.setAndPrint(23, "parseReturn");
	try
	{
		returnVal = std::stoi(word);
	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "parseReturn");;
	}
	if (!(ss >> word))
		err.setAndPrint(22, "parseReturn");
	lctn.getReturns().insert(std::make_pair(returnVal, word));
	if (ss >> word)
		err.setAndPrint(19, "parseReturn"); 
}

/* <----------------------------------------------> */

void WebServer::printAll()
{
	std::vector<Server*>::iterator ite = _parsedServers.end();
	std::vector<Server*>::iterator it = _parsedServers.begin();

	for (int i = 1; it != ite; ++it, ++i)
	{
		std::cout << "Server " << "-> "<< i << " <-" << " : \n";
		std::cout << "listen : on host '" << (*it)->getHost() << "', port '" << (*it)->getPort() << "'" << std::endl;
		for (std::vector<std::string>::const_iterator namesIt = (*it)->getServerName().begin(); namesIt != (*it)->getServerName().end(); ++namesIt)
			std::cout << "serverName: " << *namesIt << std::endl;
		for (std::map<std::string, std::string>::iterator namesIt = (*it)->getConfigMembers().getCgis().begin(); namesIt != (*it)->getConfigMembers().getCgis().end(); ++namesIt)
			std::cout << "cgi: " << namesIt->first << " " << namesIt->second << std::endl;
		std::cout << "root: " << (*it)->getConfigMembers().getRoot() << std::endl;
		for (std::vector<std::string>::const_iterator namesIt = (*it)->getConfigMembers().getIndex().begin(); namesIt != (*it)->getConfigMembers().getIndex().end(); ++namesIt)
			std::cout << "index: " << *namesIt << std::endl;
		std::cout << "max_client_body_size: " << (*it)->getConfigMembers().getMaxClientBodySize() << std::endl;
		for (std::map<int, std::string>::iterator namesIt = (*it)->getConfigMembers().getErrorPage().begin(); namesIt != (*it)->getConfigMembers().getErrorPage().end(); ++namesIt)
			std::cout << "error_page: " << namesIt->first << " " << namesIt->second << std::endl;
		if((*it)->getConfigMembers().getAutoIndex() == 1)
			std::cout << "auto_index: " << "on" << std::endl;
		else
			std::cout << "auto_index: " << "off" << std::endl;
		const std::vector<Location *> &locations = (*it)->getLocations();
		for (std::vector<Location *>::const_iterator lit = locations.cbegin(); lit != locations.cend(); ++lit){
			std::cout << "location uri: " << (*lit)->getUri() << std::endl;//***********************
 			for(std::vector<std::string>::iterator allowedIt = (*lit)->getAllowedMethods().begin(); allowedIt != (*lit)->getAllowedMethods().end(); ++allowedIt)
			{
				std::cout << "allowed_methods: " << *allowedIt << " " << std::endl;
			}
		}
	}
}



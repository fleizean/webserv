#include "../../includes/Config.hpp"

Config::Config() : _configContent(), serverBlock(false), mainBlock(true), locationBlock(false) {}

Config::Config(Config const &rhs) : _configContent(rhs._configContent), serverBlock(rhs.serverBlock), mainBlock(rhs.mainBlock), locationBlock(rhs.locationBlock) {}

Config::~Config() 
{
	for (std::vector<ServerMembers*>::iterator it = _parsedServers.begin(); it != _parsedServers.end(); ++it)
    {
        delete (*it);
    }
    _parsedServers.clear();
    
    // Location nesnelerini temizle
    for (std::vector<ServerMembers*>::iterator it = _parsedServers.begin(); it != _parsedServers.end(); ++it)
    {
        std::vector<Location*>& locations = (*it)->getLocations();
        for (std::vector<Location*>::iterator locIt = locations.begin(); locIt != locations.end(); ++locIt)
        {
            delete (*locIt);
        }
        locations.clear();
    }
}

Config &Config::operator=(Config const &rhs)
{
	_configContent	= rhs._configContent;
	serverBlock		= rhs.serverBlock;
	mainBlock		= rhs.mainBlock;
	locationBlock	= rhs.locationBlock;
	return *this;
}

std::vector<ServerMembers*>& Config::getConfig() { return this->_parsedServers; }

void Config::FileChecker(const string &conf_path)
{
	Error err(0);
	string contentsConfig;
	std::ifstream conf(conf_path);
	if(!conf)
		err.setAndPrint(1, "Config::FileChecker");
	conf.close();
	if (!conf_path.length())
		err.setAndPrint(5, "Config::FileChecker");
	if (conf_path.size() < 6 || conf_path.substr(conf_path.size() - 5) != ".conf")
		err.setAndPrint(4, "Config::FileChecker");
	if (fileToString(conf_path, contentsConfig) != -1)
		contentsConfig = removeComments(contentsConfig);
	else
		err.setAndPrint(2, "Config::FileChecker");
	if(!isBracketBalanced(contentsConfig))
		err.setAndPrint(7, "Config::FileChecker");
	this->_configContent = contentsConfig;
	parse_server();
}

/* <---------------> Before Parsing Area <---------------> */

void Config::split_server(std::string configContent)
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
		err.setAndPrint(26, "Config::split_server");
	if (_parsedServers.empty())
		err.setAndPrint(27, "Config::split_server");
}

void Config::endScopeConf()
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


void Config::parseMainArea(std::string& line)
{
	// Server new_server;
	Error err(0);
	if (line != "server {")
		err.setAndPrint(8, "Config::parseMainArea");
	_parsedServers.push_back(new ServerMembers());
	this->serverBlock = true;
	this->mainBlock = false;
}

void Config::parseServerArea(std::string& line)
{
	Error err(0);
	
	if (line.back() != ';' && line.substr(0, 8) != "location")
		err.setAndPrint(9, "Config::parseServerArea");
	if (line.back() == ';')
		line.pop_back();

	std::string			word;
	std::stringstream	ss(line);
	ServerMembers *srvr =		_parsedServers.back();
	
	ss >> word;
	
	if (word == "listen")
		parseListen(ss, *srvr);
	else if (word == "server_name")
		parseServerName(ss, *srvr);
	else if (word == "cgi_param")
		parseCgi(ss, srvr->getConfigMembers());
	else if (word == "root")
		parseRoot(ss, srvr->getConfigMembers());
	else if (word == "index")
		parseIndex(ss, srvr->getConfigMembers());
	else if (word == "auto_index")
		parseAutoIndex(ss, srvr->getConfigMembers());
	else if (word == "error_page")
		parseErrorPage(ss, srvr->getConfigMembers());
	else if (word == "allow_methods")
		parseAllowedMethods(ss, srvr->getConfigMembers());
	else if (word == "max_client_body_size")
		parseMaxClientBodySize(ss, srvr->getConfigMembers());
	else if (word == "location")
		parseLocation(ss, *srvr);
	else if (word == "upload_pass")
		parseUpload(ss, *srvr);

}

void Config::parseLocationArea(std::string& line)
{
	Error err(0);
	if (line.back() != ';')
	{
		std::cout << "line: " << line << std::endl;
		err.setAndPrint(10, "Config::parseLocationArea");
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
	else if (word == "allow_methods")
		parseAllowedMethods(ss, lctn->getConfigMembers());
	else if (word == "return")
		parseReturn(ss, *lctn);
	else if (word == "error_page")
		parseErrorPage(ss, lctn->getConfigMembers());
	else if (word == "auto_index")
		parseAutoIndex(ss, lctn->getConfigMembers());
	else if (word == "max_client_body_size")
		parseMaxClientBodySize(ss, lctn->getConfigMembers());
	else if (word == "index")
		parseIndex(ss, lctn->getConfigMembers());
	else if (word == "cgi_param")
		parseCgi(ss, lctn->getConfigMembers());
	else
		err.setAndPrint(19, "Config::parseLocationArea");
}

void Config::parse_server()
{

	split_server(this->_configContent);
	
}

/* <-----------------------------------------------------> */



/* <---------------> Parsing Area <---------------> */

void Config::parseListen(std::stringstream& ss, ServerMembers &srvr)
{
	Error err(0);
	std::string		word;
	size_t          founded_indx;

	if (!(ss >> word))
		err.setAndPrint(11, "Config::parseListen");
	founded_indx = word.find(":");
	if (founded_indx != std::string::npos)
	{
		srvr.setHost(word.substr(0, founded_indx));
		if (srvr.getHost() == "localhost" || srvr.getHost() == "0" || srvr.getHost() == "*")
			srvr.setHost("127.0.0.1");
		srvr.setHost(strToIp(word.substr(0, founded_indx)));
		word = word.substr(founded_indx + 1, word.size());
	}
	try
	{
		srvr.setPort(std::stoi(word));

	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "Config::parseListen");
	}
	
}

void Config::parseServerName(std::stringstream& ss, ServerMembers &srvr)
{
	Error err(0);
	std::string word;

	while (ss >> word) // birden fazla gelmesi gereksiz olabilir örn: localhost deneme.com...
	{
		srvr.setServerName(word); // 2130706433 localhost unsigned int değeri atama yapmak için kullanmak zorundayız
	}
	if(srvr.getServerName().empty())
		err.setAndPrint(12, "Config::parseServerName");
}

void Config::parseUpload(std::stringstream& ss, ServerMembers &srvr)
{
	Error err(0);
	std::string word;
	
	if(!(ss >> word))
		err.setAndPrint(11, "Config::parseUpload");
	srvr.setUpload(word);
	if(ss >> word)
		err.setAndPrint(19, "Config::parseUpload");
}

void Config::parseCgi(std::stringstream& ss, ConfigMembers &cm)
{
	Error err(0);
	std::string word;
	std::string cgi;
	if (!(ss >> word))
		err.setAndPrint(18, "Config::parseCgi");
	cgi = word;
	if (!(ss >> word))
		err.setAndPrint(22, "Config::parseErrorPage");
	cm.getCgi().insert(std::make_pair(cgi, word));
	if (ss >> word)
		err.setAndPrint(19, "Config::parseCgi");
}

void Config::parseRoot(std::stringstream& ss, ConfigMembers &cm)
{
	Error err(0);
	std::string word;
	
	if (!(ss >> word))
		err.setAndPrint(13, "Config::parseRoot");
	
	cm.setRoot(word);
	if (ss >> word)
		err.setAndPrint(14, "Config::parseRoot");
}

void Config::parseIndex(std::stringstream& ss, ConfigMembers& cm)
{
	Error err(0);
	std::string word;

	while(ss >> word)
	{
		cm.getIndex().push_back(word);
	}
	if (cm.getIndex().empty())
		err.setAndPrint(16, "Config::parseIndex");
}

void Config::parseMaxClientBodySize(std::stringstream& ss, ConfigMembers& cm)
{
	Error err(0);
	std::string word;

	if (!(ss >> word))
		err.setAndPrint(20, "Config::parseMaxClientBodySize");
	try
	{
		cm.setMaxClientBodySize(std::stoi(word));
	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "Config::parseMaxClientBodySize");    
	}
}

void Config::parseAutoIndex(std::stringstream& ss, ConfigMembers& cm)
{
	Error err(0);
	std::string		word;

	if(!(ss >> word))
		err.setAndPrint(28, "Config::parseAutoIndex");
	if (word == "on")
		cm.setAutoIndex(true);
	else if (word == "off")
		cm.setAutoIndex(false);
	else
		err.setAndPrint(28, "Config::parseAutoIndex");
	
	if (ss >> word)
		err.setAndPrint(28, "Config::parseAutoIndex");
}


void Config::parseErrorPage(std::stringstream& ss, ConfigMembers& cm)
{
	Error err(0);
	std::string word;
	size_t		error;

	if (!(ss >> word))
		err.setAndPrint(17, "Config::parseErrorPage");
	try
	{
		error = std::stoi(word);
	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "Config::parseErrorPage");;
	}
	if (!(ss >> word))
		err.setAndPrint(22, "Config::parseErrorPage");
	cm.getErrorPage().insert(std::make_pair(error, word));
	if (ss >> word)
		err.setAndPrint(19, "Config::parseErrorPage"); 
}


void Config::parseLocation(std::stringstream& ss, ServerMembers &srvr)
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

void Config::parseAllowedMethods(std::stringstream& ss, ConfigMembers &cm)
{
	Error err(0);
	std::string word;

	while(ss >> word)
	{
		if (!isValidMethod(word))
			err.setAndPrint(24, "Config::parseAllowedMethods");
		cm.getAllowedMethods().push_back(word);

	}
	if(cm.getAllowedMethods().empty())
		err.setAndPrint(25, "Config::parseAllowedMethods");
}


void Config::parseReturn(std::stringstream& ss, Location &lctn)
{
	Error err(0);
	std::string word;
	size_t		returnVal;

	if (!(ss >> word))
		err.setAndPrint(23, "Config::parseReturn");
	try
	{
		returnVal = std::stoi(word);
	}
	catch (std::exception& e)
	{
		err.setAndPrint(21, "Config::parseReturn");;
	}
	if (!(ss >> word))
		err.setAndPrint(22, "Config::parseReturn");
	lctn.getReturns().insert(std::make_pair(returnVal, word));
	if (ss >> word)
		err.setAndPrint(19, "Config::parseReturn"); 
}

/* <----------------------------------------------> */

void Config::printAll()
{
	std::vector<ServerMembers*>::iterator ite = _parsedServers.end();
	std::vector<ServerMembers*>::iterator it = _parsedServers.begin();

	for (int i = 1; it != ite; ++it, ++i)
	{
		if(i != 1)
			std::cout << std::endl;
		std::cout << BOLD_YELLOW << "Server " << "-> "<< i << " <-" << " : \n" << RESET;
		std::cout << GREEN;
		std::cout << "listen : on host '" << (*it)->getListen().host << " & " << (*it)->getHost() << "', port '" << (*it)->getListen().port << "'" << std::endl;
		for (std::vector<std::string>::const_iterator namesIt = (*it)->getServerName().begin(); namesIt != (*it)->getServerName().end(); ++namesIt)
			std::cout << "serverName: " << *namesIt << std::endl;
		for (std::map<std::string, std::string>::iterator namesIt = (*it)->getConfigMembers().getCgi().begin(); namesIt != (*it)->getConfigMembers().getCgi().end(); ++namesIt)
			std::cout << "cgi_param: " << namesIt->first << " " << namesIt->second << std::endl;
		if(!(*it)->getConfigMembers().getRoot().empty())
			std::cout << "root: " << (*it)->getConfigMembers().getRoot() << std::endl;
		for (std::vector<std::string>::const_iterator namesIt = (*it)->getConfigMembers().getIndex().begin(); namesIt != (*it)->getConfigMembers().getIndex().end(); ++namesIt)
			std::cout << "index: " << *namesIt << std::endl;
		for(std::vector<std::string>::iterator allowedIt = (*it)->getConfigMembers().getAllowedMethods().begin(); allowedIt != (*it)->getConfigMembers().getAllowedMethods().end(); ++allowedIt)
			std::cout << "allowed_methods: " << *allowedIt << " " << std::endl;
		std::cout << "max_client_body_size: " << (*it)->getConfigMembers().getMaxClientBodySize() << std::endl;
		for (std::map<int, std::string>::iterator namesIt = (*it)->getConfigMembers().getErrorPage().begin(); namesIt != (*it)->getConfigMembers().getErrorPage().end(); ++namesIt)
			std::cout << "error_page: " << namesIt->first << " " << namesIt->second << std::endl;
		if((*it)->getConfigMembers().getAutoIndex() == 1)
			std::cout << "auto_index: " << "on" << std::endl;
		else
			std::cout << "auto_index: " << "off" << std::endl;
		const std::vector<Location *> &locations = (*it)->getLocations();
		std::cout << RESET;
		for (std::vector<Location *>::const_iterator lit = locations.cbegin(); lit != locations.cend(); ++lit){
			std::cout << BOLD_RED << "location uri: " << (*lit)->getUri() << RESET << std::endl;
			std::cout << BLUE;
 			for(std::vector<std::string>::iterator allowedIt = (*lit)->getConfigMembers().getAllowedMethods().begin(); allowedIt != (*lit)->getConfigMembers().getAllowedMethods().end(); ++allowedIt)
			{
				std::cout << "allowed_methods: " << *allowedIt << " " << std::endl;
			}
			const std::map<int, std::string> &returnType = (*lit)->getReturns();
			for (std::map<int, std::string>::const_iterator rit = returnType.cbegin(); rit != returnType.cend(); ++rit)
			{
			    std::cout << "returns: " << rit->first << " " << rit->second << '\n';
			}
			if(!(*lit)->getConfigMembers().getRoot().empty())
				std::cout << "root:" << (*lit)->getConfigMembers().getRoot() << std::endl;
			std::cout << "max_client_body_size: " << (*lit)->getConfigMembers().getMaxClientBodySize() << std::endl;
			for (std::map<int, std::string>::iterator namesIt = (*lit)->getConfigMembers().getErrorPage().begin(); namesIt != (*lit)->getConfigMembers().getErrorPage().end(); ++namesIt)
				std::cout << "error_page: " << namesIt->first << " " << namesIt->second << std::endl;
			for (std::map<std::string, std::string>::iterator namesIt = (*lit)->getConfigMembers().getCgi().begin(); namesIt != (*lit)->getConfigMembers().getCgi().end(); ++namesIt)
				std::cout << "cgi_param: " << namesIt->first << " " << namesIt->second << std::endl;
			if((*lit)->getConfigMembers().getAutoIndex() == 1)
				std::cout << "auto_index: " << "on" << std::endl;
			else
				std::cout << "auto_index: " << "off" << std::endl;
			for (std::vector<std::string>::const_iterator namesIt = (*lit)->getConfigMembers().getIndex().begin(); namesIt != (*lit)->getConfigMembers().getIndex().end(); ++namesIt)
				std::cout << "index: " << *namesIt << std::endl;
		}
		std::cout << RESET;
	}
}
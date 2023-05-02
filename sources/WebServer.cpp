#include "../includes/WebServer.hpp"

WebServer::WebServer() : _configContent(), serverBlock(false), mainBlock(true), locationBlock(false) {}

WebServer::WebServer(WebServer const &rhs) : _configContent(rhs._configContent), serverBlock(rhs.serverBlock), mainBlock(rhs.mainBlock), locationBlock(rhs.locationBlock) {}

WebServer::~WebServer() {}

WebServer &WebServer::operator=(WebServer const &rhs)
{
    _configContent = rhs._configContent;
    serverBlock = rhs.serverBlock;
    mainBlock = rhs.mainBlock;
    locationBlock = rhs.locationBlock;
    return *this;
}


void WebServer::FileChecker(const string &conf_path)
{
    Error err(0);
	string contentsConfig;
    std::ifstream conf(conf_path);
    if(!conf){
        err.setAndPrint(1);
        exit(1);
    }
    conf.close();
    if (!conf_path.length()){
        err.setAndPrint(5);
        exit(1);
    }
    if (conf_path.size() < 6 || conf_path.substr(conf_path.size() - 5) != ".conf"){
        err.setAndPrint(4);
        exit(1);
    }
    if (fileToString(conf_path, contentsConfig) != -1)
		contentsConfig = removeComments(contentsConfig);
    else
        err.setAndPrint(2);
    if(!isBracketBalanced(contentsConfig))
        err.setAndPrint(7);
    this->_configContent = contentsConfig;
    parse_server();
}


void WebServer::split_server(std::string configContent)
{
    Error err(0);
	std::string str = "";
    int m_curr_line = 0;
    std::string     line;
    std::istringstream istreamfs(configContent);

    while (std::getline(istreamfs, line))
    {
        std::cout << "mcurrent: " << m_curr_line << ". " << line << " \nmxtx : " << this->mainBlock << " " << this->serverBlock << " " << this->locationBlock << std::endl;
        m_curr_line++;
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

void WebServer::parseServerArea(std::string& line)
{
    Error err(0);
    if (line.back() != ';' && line.substr(0, 8) != "location")
    {
		err.setAndPrint(1);
        exit(1);
    }
	if (line.back() == ';')
		line.pop_back();

	std::string			word;
	std::stringstream	ss(line);

    ss >> word;
    if (word == "location") {
        this->locationBlock = true;
        this->mainBlock = false;
        this->serverBlock = false;
    }
}

void WebServer::parseLocationArea(std::string& line)
{
    (void)line;
}

void WebServer::parseMainArea(std::string& line)
{
    Error err(0);
    if (line != "server {")
    {
        std::cout << line << std::endl;
        err.setAndPrint(8);
        exit(1);
    }
    this->serverBlock = true;
    this->mainBlock = false;

}

void WebServer::parse_server()
{
	split_server(this->_configContent);
	
	// burda aldığımız içerikleri bir server structının içine parçalarız diye düşündüm 
	// ama neler gerekiyor hatta bu struct gerekiyor mu bilmediğim için dokunmadım
}




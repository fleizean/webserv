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
		err.setAndPrint(9, "NULL");
	if (line.back() == ';')
		line.pop_back();

	std::string			word;
	std::stringstream	ss(line);

    ss >> word;
    if (word == "listen")
        parseListen(ss);
    else if (word == "server_name")
        parseServerName(ss);
    else if (word == "cgi")
        parseCgi(ss);
    else if (word == "root")
        parseRoot(ss);
    else if (word == "index")
        parseIndex(ss);
    else if (word == "error_page")
        parseErrorPage(ss);
    else if (word == "max_client_body_size")
        parseMaxClientBodySize(ss);
    else if (word == "location")
        parseLocation(ss);
}

void WebServer::parseLocationArea(std::string& line)
{
    Error err(0);
    if (line.back() != ';')
    {
        err.setAndPrint(10, "Location area");
    }
    line.pop_back();


    std::cout << "location: " << line << std::endl;

    // bak burası locationın içini parse ettiğimiz yer
    // bak locationın içindekiler geliyor altta terminalde belli isimleri
}

void WebServer::parseMainArea(std::string& line)
{
    Error err(0);
    if (line != "server {")
        err.setAndPrint(8, "NULL");
    this->serverBlock = true;
    this->mainBlock = false;
}

void WebServer::parse_server()
{
	split_server(this->_configContent);
	
	// burda aldığımız içerikleri bir server structının içine parçalarız diye düşündüm 
	// ama neler gerekiyor hatta bu struct gerekiyor mu bilmediğim için dokunmadım
}

/* <-----------------------------------------------------> */



/* <---------------> Parsing Area <---------------> */

void WebServer::parseListen(std::stringstream& ss) // bitti
{
    Error err(0);
    std::string		word;
    size_t          founded_indx;
    int             num = 0;

    if (!(ss >> word))
        err.setAndPrint(11, "listen");
    founded_indx = word.find(":");
    if (founded_indx != std::string::npos)
	{
		cout << word.substr(0, founded_indx) << endl;
		word = word.substr(founded_indx + 1, word.size());
	}
    try
    {
        num = std::stoi(word);
        cout << "listen: " << num << endl;
    }
    catch (std::exception& e)
    {
        err.setAndPrint(21, "listen");
    }
}

void WebServer::parseServerName(std::stringstream& ss) // bitti
{
    Error err(0);
    std::string word;
    size_t founded = 0;

    while (ss >> word)
    {
        if(word == "localhost")
            std::cout << word << ": 127.0.0.1" << endl;
        else
            std::cout << word << endl;
        founded += 1;
    }
    std::cout << "founded: " << founded << endl;
    if(founded == 0)
        err.setAndPrint(12, "Server name");
}

void WebServer::parseCgi(std::stringstream& ss) // bitti gibi
{
    Error err(0);
    std::string word;
    std::string file;

    if (!(ss >> word) || !(ss >> file))
        err.setAndPrint(18, "Cgi");
    std::cout << "cgi: " << file << std::endl;
    if (ss >> word)
        err.setAndPrint(19, "Cgi");
}

void WebServer::parseRoot(std::stringstream& ss) // bitti
{
    Error err(0);
    std::string word;
    
    if (!(ss >> word))
        err.setAndPrint(13, "Root");
    
    std::cout << "root: " << word << std::endl;
    if (ss >> word)
        err.setAndPrint(14, "Root");
}

void WebServer::parseIndex(std::stringstream& ss) // bitti
{
    Error err(0);
    std::string word;
    size_t  founded = 0;

    while(ss >> word){
        std::cout << "index: " << word << std::endl;
        founded++; // şimdilik founded++ ile yapıyoruz fakat ileride değişecek
    }   
    if(founded == 0)
        err.setAndPrint(16, "Index");
}

void WebServer::parseErrorPage(std::stringstream& ss) // bakılıyor
{
    Error err(0);
    std::string word;
    size_t      error;

    if (!(ss >> word))
        err.setAndPrint(17, "Error page");
    try
    {
        error = std::stoi(word);
    }
    catch (std::exception& e)
    {
        err.setAndPrint(21, "Error page");;
    }
    if (!(ss >> word))
        err.setAndPrint(22, "Error page");
    cout << "error_page: " << word << endl;
    if (ss >> word)
        err.setAndPrint(23, "Error page"); 
}

void WebServer::parseMaxClientBodySize(std::stringstream& ss)
{
    Error err(0);
    std::string word;
    int     num = 0;

    if (!(ss >> word))
        err.setAndPrint(20, "Max client body size");

    try{
        num = std::stoi(word);
        cout << "max_client_body_size: " << num << endl;
    }
    catch (std::exception& e)
    {
        err.setAndPrint(21, "Max client body size");    
    }
}

void WebServer::parseLocation(std::stringstream& ss)
{
    std::string		word;
    this->locationBlock = true;
    this->mainBlock = false;
    this->serverBlock = false;
    
    ss >> word;
    ss >> word;
    std::cout << "location name: " << word << std::endl;
    // burasıda dinleyeceğimiz adresin adı oluyor atıyorum /furkan
    // bundan gelen istekleri get set gibi şeyler olarak dinleyebiliyoruz
    // get yaparsak oradan gelecek bir request isteğini dinlemeye başlayacağız
    // request ile adam sana bir video uploadlayabilir veya fotoğraf yükleyebilir örnek olarak
}

/* <----------------------------------------------> */


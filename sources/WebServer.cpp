#include "../includes/WebServer.hpp"

WebServer::WebServer() : _configContent() {}

WebServer::WebServer(WebServer const &rhs) : _configContent(rhs._configContent) {}

WebServer::~WebServer() {}

WebServer &WebServer::operator=(WebServer const &rhs)
{
    _configContent = rhs._configContent;
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


std::vector<std::string> WebServer::split_server(std::string configContent) // bu fonksiyon config dosyasındaki server scoplarının içeriğini 
{																  // iki boyutlu bir dizeye sırasıyla aktarıyor 
	std::string str = "";										  // yalnız şöyle bir şey var tüm scopları alıyor sadece server diye değil
	std::vector<std::string> server_blocks;						  // şuan şart mı bilmediğim için eklemedim ama yaparız onu önemli değil
	int scops = 0;
	bool isOn = false;

    for (string::const_iterator it = configContent.begin(); it != configContent.end(); ++it)
    {
    	char c = *it;
    	if (c == '{' && isOn == false)
    	{
    		isOn = true;
    	}
    	else if(c == '{')
    	{
    		scops++;
    	}
    	else if(c == '}' && scops > 0)
    	{
    		scops--;
    	}
    	else if(c == '}' && scops == 0)
    	{
    		str += c;
    		isOn = false;
    		cout << str << endl;
    		server_blocks.push_back(str);
    		str = "";
    	}
    	if(isOn)
    		str += c;
    }
    return server_blocks;
}


void WebServer::parse_server()
{
	std::vector<std::string> server_blocks = split_server(this->_configContent);
	
	// burda aldığımız içerikleri bir server structının içine parçalarız diye düşündüm 
	// ama neler gerekiyor hatta bu struct gerekiyor mu bilmediğim için dokunmadım
}




/* void WebServer::BracketChecker()
{
    string line;
    vector<string> myVec;
    
    ifstream file(get_conf_path());
    while(getline(file, line){
        myVec.push_back(line);
    }
    file.close();

    for(int i = 0; i < myVec.size(); i++;)
    {
        
    }
} */
#include "../includes/WebServer.hpp"

WebServer::WebServer() : _conf_path() {}

WebServer::WebServer(WebServer const &rhs) : _conf_path(rhs._conf_path) {}

WebServer::~WebServer() {}

WebServer &WebServer::operator=(WebServer const &rhs)
{
    _conf_path = rhs._conf_path;
    return *this;
}


void WebServer::FileChecker(const std::string &conf_path)
{
	std::string contentsConfig;
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
        err.setAndPrint(5); // düzenlenecek error
    
}






/* void WebServer::BracketChecker()
{
    std::string line;
    std::vector<std::string> myVec;
    
    ifstream file(get_conf_path());
    while(getline(file, line){
        myVec.push_back(line);
    }
    file.close();

    for(int i = 0; i < myVec.size(); i++;)
    {
        
    }
} */
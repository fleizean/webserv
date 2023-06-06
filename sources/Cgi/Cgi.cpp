#include "../../includes/Cgi.hpp"

Cgi::Cgi()
{
    _envp = NULL;
    _requestHeader = "";
    _request = NULL;
    _path = "";
    _postValues = std::vector<std::string>();
    _fileName = "";
    _matchedServer = NULL;
}

Cgi::Cgi(char** envp, std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues, ServerMembers* matchedServer) 
{
    _envp = envp;
    _requestHeader = m_request;
    _request = req;
    _path = path;
    _postValues = postValues;
    _fileName = fileName;
    _matchedServer = matchedServer;
}

Cgi::~Cgi() {}

void Cgi::extractKeyValues() {
    for(std::vector<std::string>::iterator it = _postValues.begin(); it != _postValues.end(); it++)
        _env.push_back(*it);
}

void Cgi::initOthersEnvironment(char* cwd)
{
    std::string serverName = "";
    for (std::vector<std::string>::const_iterator it = _matchedServer->getServerName().begin(); it != _matchedServer->getServerName().end(); ++it) {
        serverName = *it;
        break;
    }
    _env.push_back("CONTENT_TYPE=" + _request.getFirstMediaType());
    _env.push_back("DOCUMENT_ROOT");
    _env.push_back("CONTENT_LENGTH=" + std::to_string(_request.getContentLength()));
    _env.push_back("HTTP_COOKIE=none");
    _env.push_back("HTTP_HOST=off");
    _env.push_back("HTTP_REFERER");
    _env.push_back("HTTP_USER_AGENT");
    _env.push_back("HTTPS=off");
    _env.push_back("PATH=" + std::string(cwd));
    _env.push_back("QUERY_STRING");
    _env.push_back("REMOTE_ADDR=127.0.0.1");
    _env.push_back("REMOTE_HOST");
    _env.push_back("REMOTE_PORT=");
    _env.push_back("REMOTE_USER");
    _env.push_back("REQUEST_METHOD=" + _request.getMethod());
    _env.push_back("REQUEST_URI=" + std::string(cwd));
    _env.push_back("SCRIPT_FILENAME"); 
    _env.push_back("SCRIPT_NAME");
    _env.push_back("SERVER_ADMIN");
    _env.push_back("SERVER_NAME=" + serverName);
    _env.push_back("SERVER_PORT=" + std::to_string(_request.getPort()));
    _env.push_back("SERVER_SOFTWARE=Weebserv/1.0");
    _env.push_back("SERVER_PROTOCOL=" + _request.getProtocol());
    _env.push_back("REDIRECT_STATUS=200");

    /* std::cout << BOLD_YELLOW <<"\n----------> Env Testing <----------\n" << RESET;
    std::cout << CYAN << std::endl;
    for(std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++)
        std::cout << *it << std::endl;
    std::cout << RESET << std::endl; */
}

std::string Cgi::cgiExecute()
{
    size_t i = 0;
    int pip[2];
    pid_t child = 0;
    pid_t parent = 0;
    char* cwd;
    Error err(0);
    extractKeyValues();
    cwd = get_cwd_buf();
    initOthersEnvironment(cwd);
    
    int fd = open("tmp", O_CREAT | O_TRUNC | O_WRONLY | O_NONBLOCK, 0777);
    write(fd, _requestHeader.c_str(), _requestHeader.size());
    close(fd);
    free(cwd);
    char **yes = new char *[_env.size() + 1];
    for (; i != _env.size(); i++)
        yes[i] = (char *)_env.at(i).c_str();
    yes[i] = NULL;
    char *echo[3] = {(char *)"cat", (char *)"tmp", NULL};
    char *cmd[] =  {&_path[0], &_fileName[0], NULL};
    
    if (pipe(pip) == -1)
    {
        perror("CGI part : Pipe failed");
        exit(1);
    }
    child = fork();

    int tmp = open(".tmp", O_CREAT | O_TRUNC | O_NONBLOCK | O_RDWR, 0777);

    if (child == -1)
    {
        std::cerr << "Fork failed" << std::endl;
		return ("Status: 500\r\n\r\n");
    }
    else if (!child)
    {
        dup2(pip[1], 1);
        close(pip[0]);
        execve("/bin/cat", echo, NULL);
    }
    else
    {
        int status2;

        wait(&status2);
        parent = fork();
        if (!parent)
        {
            dup2(pip[0], 0);
            dup2(tmp, 1);
            close(pip[1]);
            execve(cmd[0], cmd, yes);
        }
        else
        {
            int status;

            wait(&status);
            close(pip[0]);
            close(pip[1]);
            close(tmp);

            usleep(100000);

            tmp = open(".tmp", O_NONBLOCK | O_RDONLY);

            char buf[65535];
            bzero(buf, 65535);
            read(tmp, buf, 65535);
            close(tmp);
            _requestHeader = std::string(buf);
            remove("tmp");
            remove(".tmp");

            delete[] yes;
            return (_requestHeader);
        }
    }
    delete[] yes;
    return _requestHeader;
}

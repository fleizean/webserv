#include "../../includes/Cgi.hpp"

Cgi::Cgi()
{
    _envp = NULL;
    _requestHeader = "";
    _request = NULL;
    _path = "";
    _postValues = std::vector<std::string>();
    _fileName = "";
}

Cgi::Cgi(char** envp, std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues) 
{
    _envp = envp;
    _requestHeader = m_request;
    _request = req;
    _path = path;
    _postValues = postValues;
    _fileName = fileName;
}

Cgi::~Cgi() 
{
}

void Cgi::extractKeyValues() {
    for(std::vector<std::string>::iterator it = _postValues.begin(); it != _postValues.end(); it++)
        _env.push_back(*it);
}

void Cgi::initOthersEnvironment(char* cwd)
{
    _env.push_back("CONTENT_TYPE text/html");
    _env.push_back("DOCUMENT_ROOT");
    _env.push_back("CONTENT_LENGTH");
    _env.push_back("HTTP_COOKIE = none");
    _env.push_back("HTTP_HOST = off");
    _env.push_back("HTTP_REFERER");
    _env.push_back("HTTP_USER_AGENT");
    _env.push_back("HTTPS = off");
    _env.push_back("PATH" + std::string(cwd));
    _env.push_back("QUERY_STRING");
    _env.push_back("REMOTE_ADDR");
    _env.push_back("REMOTE_HOST");
    _env.push_back("REMOTE_PORT");
    _env.push_back("REMOTE_USER");
    _env.push_back("REQUEST_METHOD");
    _env.push_back("REQUEST_URI" + std::string(cwd));
    _env.push_back("SCRIPT_FILENAME" ); 
    _env.push_back("SCRIPT_NAME");
    _env.push_back("SERVER_ADMIN");
    _env.push_back("SERVER_NAME");
    _env.push_back("SERVER_PORT");
    _env.push_back("SERVER_SOFTWARE");
    _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    _env.push_back("REDIRECT_STATUS=200");
}


std::string Cgi::cgiExecute()
{
    char cwd[256];
    Error err(0);
    extractKeyValues();
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        err.setAndPrint(51, "Cgi::cgiExecute");
    initOthersEnvironment(cwd);
     
    int pip[2];
    int ret = 0;
    pid_t child = 0;
    pid_t parent = 0;
	int checke_prob = 0;
    int fd = open("tmp", O_CREAT | O_TRUNC | O_WRONLY | O_NONBLOCK, 0777);
    write(fd, _requestHeader.c_str(), _requestHeader.size());
    close(fd);
    size_t i = 0;

    char **yes = new char *[_env.size() + 1];
    for (; i != _env.size(); i++)
        yes[i] = (char *)_env.at(i).c_str();
    yes[i] = NULL;

    char *echo[3] = {(char *)"cat", (char *)"tmp", NULL};
    std::cout << ": "<< echo[0] << echo[1] << std::endl;
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
        ret = execve("/bin/cat", echo, NULL);
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
            ret = execve(cmd[0], cmd, yes);
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
            int checkRead = read(tmp, buf, 65535);
			if (checkRead == 0)
				checke_prob = 0;
			else if (checkRead == -1)
				checke_prob = -1;
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

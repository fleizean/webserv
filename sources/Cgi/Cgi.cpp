#include "Cgi.hpp"

Cgi::Cgi() {}

Cgi::~Cgi() {}

std::string Cgi::cgiExecute(std::string location, std::string executable ,std::string	postname[], std::string	postvalue[], int j,std::string m_request, int code){
    code = 500;
    std::vector<std::string> env;
    std::string content = m_request;
    std::string combine[10000];
    for (int nm = 1; nm != j+1 ; nm++)
        combine[nm] = postname[nm]+ "=" + postvalue[nm];
    char cwd[256];
    for (int nm = 1; nm != j+1 ; nm++)
        combine[nm] = postname[nm]+ "=" + postvalue[nm];
    for (int k = 1; k != j+1 ; k++)
        env.push_back(combine[k]);;
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        exit(1);
    }

    env.push_back("CONTENT_TYPE text/html");
    env.push_back("DOCUMENT_ROOT");
    env.push_back("CONTENT_LENGTH");
    
    env.push_back("HTTP_COOKIE = none");
    env.push_back("HTTP_HOST = off");
    env.push_back("HTTP_REFERER");
    env.push_back("HTTP_USER_AGENT");
    env.push_back("HTTPS = off");
    env.push_back("PATH" + std::string(cwd));
    env.push_back("QUERY_STRING");
    
    env.push_back("REMOTE_ADDR");
    env.push_back("REMOTE_HOST");
    env.push_back("REMOTE_PORT");
    env.push_back("REMOTE_USER");
    env.push_back("REQUEST_METHOD");
    env.push_back("REQUEST_URI" + std::string(cwd));
    
    env.push_back("SCRIPT_FILENAME" ); 
    env.push_back("SCRIPT_NAME");
    
    env.push_back("SERVER_ADMIN");
    env.push_back("SERVER_NAME");
    env.push_back("SERVER_PORT");
    env.push_back("SERVER_SOFTWARE");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");

    env.push_back("REDIRECT_STATUS=200");

     int pip[2];
     int ret = 0;
    pid_t child = 0;
    pid_t parent = 0;
	int checke_prob = 0;
    int fd = open("tmp", O_CREAT | O_TRUNC | O_WRONLY | O_NONBLOCK, 0777);

    int checke_Cwrite = write(fd, content.c_str(), content.size());
	if (checke_Cwrite == 0)
	{
		checke_prob = 0;
	}
	else if (checke_Cwrite == -1)
	{
		checke_prob = -1;
	}


    close(fd);
    size_t i = 0;

    char **yes = new char *[env.size() + 1];

    for (; i != env.size(); i++)
        yes[i] = (char *)env.at(i).c_str();
    yes[i] = NULL;

    char *echo[3] = {(char *)"cat", (char *)"tmp", NULL};
    //char *cmd[] = {path, (char *)location.c_str(), NULL};
    char *cmd[] =  {&location[0], &executable[0], NULL};
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
        ret = execvp(echo[0], echo);
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
            content = std::string(buf);
            remove("tmp");
            remove(".tmp");

            delete[] yes;
            return (content);
        }
    }

    delete[] yes;
    return content;
}

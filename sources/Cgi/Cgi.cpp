#include "../../includes/CGI.hpp"


CGI::CGI() {
	this->_query = "";
	this->_body = "";
	this->_contentType = "";
}

CGI::CGI(const CGI &src) {
	if (this != &src) {
		this->_body = src._body;
		this->_env = src._env;
	}
	return ;
}

CGI	&CGI::operator=(const CGI &src) {
	if (this != &src) {
		this->_body = src._body;
		this->_env = src._env;
	}
	return *this;
}

/*
 * Set the query string of the request if this is a multipart/form-data request
 */
std::string createQueryMultipart(std::string body, std::string boundary) {
    std::string delimiter = "\r\n--" + boundary + "\r\n";
    std::string dataFiles;
    std::string multipartQuery = "";
    // Browse each part of the body
    while (body.size()) {
        dataFiles = body.substr(0, body.find(delimiter));
        // Check if the part is a field
        if (dataFiles.find("name=") != std::string::npos) {
            if (multipartQuery.size())
                multipartQuery += "&";
            std::string name = dataFiles.substr(dataFiles.find("name=") + 6);
            name = name.substr(0, name.find("\""));
            std::string value = dataFiles.substr(dataFiles.find("\r\n\r\n") + 4);
            value = value.substr(0, value.find("\r\n"));
            if (dataFiles.find("filename=") != std::string::npos) {
                value = dataFiles.substr(dataFiles.find("filename=") + 10);
                value = value.substr(0, value.find("\""));
            }
            for (size_t i = 0; i < value.size(); i++)
                if (value[i] == ' ')
                    value[i] = '+';
            multipartQuery += name + "=" + value;
        }
        dataFiles = "";
        if (body.find(delimiter) != std::string::npos)
            body = body.substr(body.find(delimiter) + delimiter.size());
        else
            break;
    }
    return multipartQuery;
}

/*
 * Create the env array for the CGI program from envp
 * Take the envp array as argument and the pathInfo and query string if needed
 * Return the new env array with PATH_INFO variable
 */
char	**CGI::_createEnv(char **envp, std::string pathInfo) const {
    // Calculate the size of the envp array
    int sizeEnvp = 0;
    while (envp[sizeEnvp])
        sizeEnvp++;
    if (_contentType != "")
        sizeEnvp += 4;
    // --- up to here
    // Check if the query string is not empty
    if (this->_query.size() > 1)
        sizeEnvp++;
    // Set the new array
    char	**env = new char*[sizeEnvp + 2];
    int i = 0;
    // --- add here
    while (i < sizeEnvp - 5) {
        env[i] = new char[strlen(envp[i]) + 1];
        env[i] = strcpy(env[i], envp[i]);
        i++;
    }
    // Add PATH_INFO variable in env
    std::string element = "PATH_INFO=" + pathInfo;
    env[i] = new char[element.size() + 1];
    env[i] = strcpy(env[i], (const char*)element.c_str());
    i++;
    // Add the QUERY variable in env if needed
    if (this->_query.size()) {
        std::string element = "QUERY_STRING=" + this->_query;
        env[i] = new char[element.size() + 1];
        env[i] = strcpy(env[i], (const char*)element.c_str());
        i++;
    }
    if (_contentType != "") {
        std::string element = "CONTENT_TYPE=" + _contentType;
        env[i] = new char[element.size() + 1];
        env[i] = strcpy(env[i], (const char*)element.c_str());
        i++;
        element = "CONTENT_LENGTH=" + _contentLength.substr(_contentLength.find(":") + 2);
        env[i] = new char[element.size() + 1];
        env[i] = strcpy(env[i], (const char*)element.c_str());
        i++;
        element = "PATH_TRANSLATED=" + _pathTranslated;
        env[i] = new char[element.size() + 1];
        env[i] = strcpy(env[i], (const char*)element.c_str());
        i++;
        element = "QUERY_STRING=" + createQueryMultipart(_body, _boundary);
        env[i] = new char[element.size() + 1];
        env[i] = strcpy(env[i], (const char*)element.c_str());
        i++;
        // --- add here

    }
    // Set the last element to NULL
    env[i] = NULL;
	return env;
}

/*
 * Execute the CGI program
 * Take two argument: the path to the CGI program and the envp arrays
 * Return the body of the response
 */
std::string	CGI::execute(const std::string& scriptName, char **envp) {
	std::string	tmpBody;
	char **env = this->_createEnv(envp, scriptName);
	if (this->_query.size())
	    this->setBody(this->_query);
	int tmpStdin = dup(STDIN_FILENO);
	int tmpStdout = dup(STDOUT_FILENO);
	FILE	*fileIn = tmpfile();
	FILE	*fileOut = tmpfile();
	long	fdIn = fileno(fileIn);
	long	fdOut = fileno(fileOut);
	int	ret = 1;
	write(fdIn, _body.c_str(), _body.size());
	lseek(fdIn, 0, SEEK_SET);
	pid_t   pid = fork();
	if (pid == -1) {
		std::cerr << "Error: fork() in execute" << std::endl;
		return ("Status: 500\r\n\r\n");
	}
	else if (!pid) {
		dup2(fdIn, STDIN_FILENO);
		dup2(fdOut, STDOUT_FILENO);
		// Create the arg array for execve
        char* const arg[] = {const_cast<char*>(scriptName.c_str()), const_cast<char*>(_body.c_str()), NULL};
        // Call the CGI script with the script and the arguments (variables)
        execve(scriptName.c_str(), arg, env);
		std::cerr << "Error: execve() in execute" << std::endl;
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else {
		char    buffer[65536] = {0};
		waitpid(-1, NULL, 0);
		lseek(fdOut, 0, SEEK_SET);
		ret = 1;
		while (ret > 0) {
			memset(buffer, 0, 65536);
			ret = read(fdOut, buffer, 65536 - 1);
			tmpBody += buffer;
		}
	}
	dup2(tmpStdin, STDIN_FILENO);
	dup2(tmpStdout, STDOUT_FILENO);
	fclose(fileIn);
	fclose(fileOut);
	close(fdIn);
	close(fdOut);
	close(tmpStdin);
	close(tmpStdout);
	for (size_t i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
	if (!pid)
		exit(0);
	return (tmpBody);
}

void	CGI::setBody(std::string body) {
	_body = body;
	return ;
}

void	CGI::setScript(std::string cgi) {
	_cgi = cgi;
	return ;
}

void	CGI::setMethod(std::string method) {
	_method = method;
	return ;
}

void	CGI::setPath(std::string file) {
	_file = file;
	return ;
}

void	CGI::setQuery(std::string query) {
	_query = query;
	return ;
}

void	CGI::setAddr(std::string host) {
	_host = host;
	return ;
}

void	CGI::setType(std::string type) {
	_type = type;
	return ;
}

void	CGI::setPort(int    port) {
	_port = port;
	return ;
}

void    CGI::setContentType(std::string contentType) {
    _contentType = contentType;
    return ;
}

void    CGI::setContentLength(std::string contentLength) {
    _contentLength = contentLength;
    return ;
}

void    CGI::setPathTranslated(std::string pathTranslated) {
    _pathTranslated = pathTranslated;
    return ;
}

void    CGI::setBoundary(std::string boundary) {
    _boundary = boundary;
    return ;
}
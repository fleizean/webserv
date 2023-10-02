#include "../../includes/Cgi.hpp"

Cgi::Cgi()
{
    _requestHeader = "";
    _cgiPath = "";
    _postValues = std::vector<std::string>();
    _fileExecutePath = "";
    _matchedServer = NULL;
    _cgiExecutePath = "";
    _multiBody = "";
    _statusCode = 200;
}

Cgi::Cgi(std::string fileName, std::string m_request, Request req, std::string path, std::vector<std::string> postValues, ServerMembers* matchedServer, std::string cgiPath, std::string multiBody) 
{
    _requestHeader = m_request;
    _request = req;
    _cgiExecutePath = path;
    _postValues = postValues;
    _fileExecutePath = fileName;
    _matchedServer = matchedServer;
    _multiBody = multiBody;
    _cgiPath = trim(cgiPath, "/");
    _statusCode = 200;
/*     std::cout << BOLD_YELLOW << "[" << m_request << "]" << RESET << std::endl; */
}

Cgi::~Cgi() {}


void Cgi::extractKeyValues() {
    for (size_t i = _postValues.size() - 1; i < _postValues.size(); ++i) { // -1 diyerek sonuncu alan key=value alanı olduğu için bu şekilde parse ettik
        _keyValue += _postValues[i];
        if (i != _postValues.size() - 1) {
            _keyValue += "&";
        }
    }
}

void Cgi::initOthersEnvironment()
{
    _env.push_back("SCRIPT_FILENAME=" + _cgiPath + _request.getLocation());
    _env.push_back("SCRIPT_NAME=" + _cgiPath + _request.getLocation());
    if (_request.getMulti() == 1)
        _env.push_back("CONTENT_TYPE=" + _request.getMultiPart());
    else
        _env.push_back("CONTENT_TYPE=" + _request.getContentType());
    _env.push_back("CONTENT_LENGTH=" + std::to_string(_request.getContentLength()));
    _env.push_back("PATH_INFO=" + _request.getLocation());
    _env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    _env.push_back("REQUEST_METHOD=" + _request.getMethod());
    _env.push_back("REQUEST_URI=" + _request.getLocation());
    _env.push_back("SERVER_PORT=" + std::to_string(_request.getPort()));
    _env.push_back("SERVER_PROTOCOL=" + _request.getProtocol());
    _env.push_back("SERVER_SOFTWARE=webserv");
    _env.push_back("REDIRECT_STATUS=200");

    /* std::cout << BOLD_YELLOW << "\n----------> Env Testing <----------\n" << RESET;
    std::cout << CYAN << std::endl;
    for(std::vector<std::string>::iterator it = _env.begin(); it != _env.end(); it++)
        std::cout << *it << std::endl;
    std::cout << RESET << std::endl; */
}

/**
 * @brief Cgi'ı çalıştıran alan
 * 
 * dup2(result_pipe[1], 1): dup2 ile standard çıkış (stdout) (1) yerine result_pipe'ın 
 * yazma ucunu (result_pipe[1]) bağlar. Böylece CGI programının çıktısı bu boruya yazılacaktır.
 * 
 * Örnek olarak, eğer ki Cgi sayfam bana bir HTML sayfası döndürüyorsa bu alan üzerinden bana gönderir.
 * 
 * dup2(body_pipe[0], 0) kullanılarak standard giriş (stdin) (0) body_pipe'ın okuma ucuna bağlanır. 
 * Bu sayede CGI programı, isteğin gövdesini body_pipe üzerinden okuyabilir.
 * 
 * Örnek olarak, bir FORM post işlevi yaptığımda bu verileri bu kanal üzerinden gönderirim.
 * Daha sonrasında Cgi bu verileri işlemek için stdin üzerinden okuma yapar.
 * 
 * @return std::string 
 */
std::string Cgi::cgiExecute()
{
    extractKeyValues();
    initOthersEnvironment();

    size_t          i = 0;
    char	        output[4096];
	int		        readed;
	int	            body_pipe[2];
	int	            result_pipe[2];
	std::string     tmp2;
	char            *av1 = (char *)this->_cgiExecutePath.c_str();;

	char            *av2;
	char            *av[3];

    av[2] = 0;

    av2 = (char *)_fileExecutePath.c_str();

    if (av1 == NULL || av2 == NULL)
        _statusCode = 500;
    av[0] = av1;
	av[1] = av2;
    char **env = new char*[_env.size() + 1];
    for (i = 0; i < _env.size(); i++)
        env[i] = (char*)_env.at(i).c_str();
    env[i] = NULL;

    pipe(body_pipe);    // Bizim cgi ile bir veriyi servera yollayacağımız durumdaki köprü. (veri iletişimi)
	pipe(result_pipe);  // Cgi'nın bir veri gönderme durumunda veriyi alacağımız köprü. (veri iletişimi)

    if (this->_request.getMethod() == "POST" && !_multiBody.empty())
		write(body_pipe[1], _multiBody.c_str(), _multiBody.length()); // image veriyi gönder 
    else if (this->_request.getMethod() == "POST" && _multiBody.empty())
        write(body_pipe[1], _keyValue.c_str(), _keyValue.length()); // fname=tes&lname=ttest&second=etset&Third=estet veriyi gönder
    close(body_pipe[1]);

    if (!fork()) // fork oluşturulduysa girecek
	{
		close(result_pipe[0]);
		dup2(result_pipe[1], 1); // CGI programının sonucunu result_pipe[1] içine yazacak
		close(result_pipe[1]);

		if (this->_request.getMethod() == "POST")
			dup2(body_pipe[0], 0); // CGI programı girdiyi body_pipe[0]'dan okuyacak
		close(body_pipe[0]);

		execve(av[0], av, env); // hata olmazsa aşağıya girmeden 120'den devam eder
        std::cerr << "Execv Err! " << std::endl;
        while(env[i])
           delete[] env[i];
        delete[] env;
		exit(-1); // child process sonlandırılır
	}
    wait(NULL); // child process sonuçlanmasını bekleriz
	close(body_pipe[0]);
	close(result_pipe[1]);

	readed = read(result_pipe[0], output, 4096);
	if (readed == 0) {
        _statusCode = 500;
		std::cout << "Cgi Read Fail!" << std::endl << std::flush;
    }
	close(result_pipe[0]);
	output[readed] = 0;
    while(env[i])
        delete[] env[i];
    delete[] env;
    return (std::string(output, readed));
}

int  Cgi::getStatusCode() { return this->_statusCode; }
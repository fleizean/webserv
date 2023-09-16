#include "../../includes/Response.hpp"

Response::Response(Request req, std::vector<ServerMembers*> servers, ServerMembers* matchedServer) : _req(req), _servers(servers), _matchedServer(matchedServer)
{
    setupRequest();
}

Response::~Response() { }

/* --------> Functions <-------- */

template <typename T>
static std::string toStringFor(T numb)
{
	std::stringstream stream;
	stream << numb;
	return stream.str();
}

void Response::run()
{
	int j = 1;
	std::string aled;
	std::string all;
	bool yes;
	std::vector<Location*>::iterator locItos = _matchedServer->getLocations().begin();

	for (std::vector<Location*>::iterator locIt = _matchedServer->getLocations().begin(); locIt != _matchedServer->getLocations().end(); ++locIt, locItos++, ++j)
	{
		if (_path.find((*locIt)->getUri()) != std::string::npos)
		{
			_uriRoot = (*locIt)->getConfigMembers().getRoot();
			aled = (*locIt)->getUri();
			_maxBody = (*locIt)->getConfigMembers().getMaxClientBodySize();
			_upload = _matchedServer->getUpload();
			for (std::map<int, std::string>::iterator errIt = (*locIt)->getConfigMembers().getErrorPage().begin(); errIt != (*locIt)->getConfigMembers().getErrorPage().end(); ++errIt)
				mp.insert(std::make_pair(std::to_string(errIt->first), errIt->second));
			// buraya gelecek return
			if ((*locIt)->getHasRedirection() == true) 
			{
				_hasRedirection = true;
				_redirectionType = (*locIt)->getRedirectionType();
				_redirectionURI = (*locIt)->getRedirectionURI();
			}
			for (std::vector<std::string>::iterator namesIt = (*locIt)->getConfigMembers().getAllowedMethods().begin(); namesIt != (*locIt)->getConfigMembers().getAllowedMethods().end(); ++namesIt)
				all += *namesIt;
 			yes = true;
		}
		else if (yes != true)
		{
			all = "GETPOSTDELETE";
			_maxBody = _matchedServer->getConfigMembers().getMaxClientBodySize();
		}
		for (std::map<std::string, std::string>::iterator namesIt = _matchedServer->getConfigMembers().getCgi().begin(); namesIt != _matchedServer->getConfigMembers().getCgi().end(); ++namesIt)
			mp.insert(std::make_pair(namesIt->first, namesIt->second));
		_upload = _matchedServer->getUpload();
	}
	for (std::vector<std::string>::const_iterator namesIt = _matchedServer->getServerName().begin(); namesIt != _matchedServer->getServerName().end(); ++namesIt)
		_serverName = *namesIt;
	std::map<std::string, std::string> indexmap;
	for (std::vector<Location*>::iterator locIt = _matchedServer->getLocations().begin(); locIt != _matchedServer->getLocations().end(); ++locIt, ++j)
		for (std::vector<std::string>::iterator namesIt = (*locIt)->getConfigMembers().getIndex().begin(); namesIt != (*locIt)->getConfigMembers().getIndex().end(); ++namesIt)
			indexmap.insert(std::make_pair((*locIt)->getUri(), *namesIt));
	if (_hasRedirection == true && _path != "/favicon.ico") {
		_path = "/" + _redirectionURI;
	}
	else {
		if (_path == aled)
		_path += indexmap[aled];
	}
	_path.replace(_path.find(aled), aled.length(), _uriRoot);
	// std::cout << "_path: " << _path << std::endl;
	// isteği yönetmek için
	processRequest();
}

void Response::processRequest()
{
	if (_type == "GET")
		getMethodes();
	else if (_type == "DELETE")
		deleteMethodes();
	else if (_type == "POST") 
		postMethodes();
	else
	{
		resetHTML();
		if (_type == "POST" || _type == "DELETE" || _type == "GET")
			_code = 405;
		else
			_code = 501;
		errorPage();
		modifyResponseHeader();
	}
}

/**
 * @brief Error Status'ların başlangıç değerlerini verir
 * 
 */	
void Response::errorStatus()
{
	_errorRep[100] = "Continue";
	_errorRep[200] = "OK";
	_errorRep[201] = "Created";
	_errorRep[204] = "No Content";
	_errorRep[400] = "Bad Request";
	_errorRep[403] = "Forbidden";
	_errorRep[404] = "Not Found";
	_errorRep[405] = "Method Not Allowed";
	_errorRep[413] = "Payload Too Large";
	_errorRep[500] = "Internal Server Error";
	_errorRep[501] = "Not Implemented";
}

/**
 * @brief Dosya yoluna dayalı olarak içerik türünü belirler.
 *
 * @param path Dosya yolunu içeren bir std::string.
 *
 * Bu fonksiyon, verilen dosya yoluna dayalı olarak içerik türünü belirler ve _contentType üye değişkenini ayarlar.
 * Fonksiyon, path parametresindeki dosya yolunun uzantısını alarak içerik türünü belirler.
 * İlgili içerik türüne göre _contentType üye değişkeni güncellenir.
 * Eğer verilen dosya yolunun uzantısı yukarıda belirtilen uzantılardan birine uymuyorsa, _contentType üye değişkenine varsayılan değer olan "text/html" atanır.
 * _cgiType üye değişkeni de _contentType ile aynı değere atanır.
 */


/**
 * @brief Belirtilen dosyanın var olup olmadığını kontrol eder ve koşullara bağlı olarak işlemler gerçekleştirir.
 *
 * @param fileName Kontrol edilecek dosyanın adı.
 * @return Dosya varlık kontrolünün sonucunu gösteren HTTP yanıt kodu.
 */
int Response::fileExist(const char* fileName)
{
    std::string line;
    _code = 200;
    int autoindx[65535];
    std::vector<ServerMembers*>::iterator ite = _servers.end();
    std::vector<ServerMembers*>::iterator it = _servers.begin();
    
    // Her sunucu yapılandırması için autoindex değerlerini al
    for (int i = 1; it != ite; ++it, ++i)
    {
        autoindx[(*it)->getListen().port] = (*it)->getConfigMembers().getAutoIndex();
    }
    
    std::ifstream document;
    // Belirtilen yol bir dosya mı diye kontrol et
    if (checkIfPathIsFile(fileName) == true)
    {
        // Dosya var mı diye kontrol et
        if ((access(fileName, F_OK) == 0))
        {
            document.open(fileName, std::ifstream::in);
            
            // Dosya açılabiliyorsa
            if (document.is_open() == false)
            {
                _code = 403;
                return _code;
            }
            
            std::stringstream test;
            test << document.rdbuf();
            _http = test.str();
            
            // Uzantı ve yorumlayıcı eşleşiyorsa CGI betiğini çalıştır
			
            if (_cgiType == "py" && mp[".py"].find("/usr/bin/python3") != std::string::npos)
            {
				Cgi _cgi(_fileName, _bando, _req, "/usr/bin/python3", _postValues, _matchedServer);
                _http = _cgi.cgiExecute();
                _code = 200;
                return _code;
            }
            else if (_cgiType == "pl" && mp[".pl"].find("/usr/bin/perl") != std::string::npos)
            {
				Cgi _cgi(_fileName, _bando, _req, "/usr/bin/perl", _postValues, _matchedServer);
                _http = _cgi.cgiExecute();
                _code = 200;
                return _code;
            }
            else if (_cgiType == "php" && mp[".php"].find("/usr/bin/php-cgi") != std::string::npos)
            {
				Cgi _cgi(_fileName, _bando, _req, "/usr/bin/php-cgi", _postValues, _matchedServer);
                _http = _cgi.cgiExecute();
                _code = 200;
                return _code;
            }
            return _code;
        }
        else
            _code = 404;
        return _code;
    }
    else if (checkIfPathIsFile(fileName) == false && _type == "GET" && autoindx[atoi(_host.substr(_host.find(":") + 1).c_str())] == 1)
    {
        // Otomatik dizin listelemesi etkinse, HTTP yanıtına otomatik dizin içeriğini ekle
        _http += fAutoIndex(fileName);
        _code = 200;
        return _code;
    }
    else
        _code = 404;
    return _code;
}

/**
 * @brief Belirtilen dizin için otomatik dizin içeriğini oluşturur.
 *
 * @param path Otomatik dizin içeriğinin oluşturulacağı dizin yolu.
 * @return Oluşturulan otomatik dizin içeriği.
 */
std::string Response::fAutoIndex(const char* path)
{
    std::string Directory(_path);
    DIR* dir = opendir(path);
    std::string Autoindex_Page =
        "<!DOCTYPE html>\n\
        <html>\n\
        <head>\n\
                <title>" +
        Directory + "</title>\n\
        </head>\n\
        <body>\n\
        <h1>INDEX</h1>\n\
        <p>\n";
    
    if (dir == NULL)
    {
        std::cerr << "Error: could not open the following path" << path << std::endl;
        return "";
    }
    
    if (Directory[0] != '/')
        Directory = "/" + Directory;
    
    // Dizin içeriğini tarayarak otomatik dizin içeriğini oluştur
    for (struct dirent* dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
    {
        Autoindex_Page += createDirectoryLink(std::string(dirEntry->d_name), Directory, _host);
    }
    
    Autoindex_Page +=
        "\
        </p>\n\
        </body>\n\
        </html>\n";
    
    closedir(dir);
    return Autoindex_Page;
}

/**
 * HTTP isteğinin POST yöntemini işler.
 * Bu fonksiyon, POST isteğini işler ve dosya yüklemeleri ile CGI yürütmesini gerçekleştirir.
 * İçerik türünü belirler, dosya yüklemelerini yönetir ve istenen dosya bir PHP veya Python betiği ise CGI betiklerini yürütür.
 * İçerik uzunluğu, izin verilen maksimum boyutu aşarsa yanıt kodunu 413 (İstek Varlık Çok Büyük) olarak ayarlar.
 * Yanıt kodu bir sunucu hatası (500) belirtiyorsa hata sayfası oluşturur.
 * Son olarak, uygun durum kodu, içerik türü, içerik uzunluğu ve konumu içeren yanıt başlığını oluşturur
 * ve HTTP yanıt gövdesine ekler.
 *
 * @return POST yönteminin başarılı bir şekilde işlendiğini belirten 0 değerini döndürür.
 */
int Response::postMethodes()
{
	_postmethod = true;
	char* point_path = realpath(".", NULL);
	std::string path = point_path + removeAll(_path, point_path);
	free(point_path);
	_code = 413;
	parseQueryString(_bando.substr(_bando.find("\r\n\r\n") + strlen("\r\n\r\n")));
	getContentType(path);
	if (_path.substr(_path.find_last_of(".") + 1) == "php"){
		Cgi _cgi(path.c_str(), _bando, _req, "/usr/bin/php", _postValues, _matchedServer);
		_http = _cgi.cgiExecute();
	}
	if (_path.substr(_path.find_last_of(".") + 1) == "py"){
		Cgi _cgi(path.c_str(), _bando, _req, "/usr/bin/python3", _postValues, _matchedServer);
		_http = _cgi.cgiExecute();
	}
	if (_contentLen <= _maxBody)
	{
		_code = 200;
		std::string sear(_bando);
		size_t i = sear.rfind("Content-Type:");
		if (i != std::string::npos)
			i = sear.find("\n", i);
		if (i != std::string::npos)
		{
			size_t j = sear.find("------WebKitFormBoundary", i);
			if (j != std::string::npos)
			{
				char* cwd;
				_isUpload = true;

				cwd = get_cwd_buf();
				std::string scwd = cwd;
				free(cwd);
				_http = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n</head>\n<h1>File " + _fileName + " has been uploaded yiğen.</h1></html>";				
				uploadFile(std::string((sear.begin() + i + 3), sear.begin() + j - 2), _bando);
			}
		}
	}
	if (_contentLen >= _maxBody)
		_code = 413;
	if (_http.find("Status: 500") != std::string::npos)
	{
		_code = 500;
		_http = "";
		errorPage();
	}
	errorPage();
	modifyResponseHeader();
	return 0;
}

/**
 * @brief Dizin girişi için otomatik dizin içeriğindeki bir bağlantı oluşturur.
 *
 * @param dirEntry Dizin girişi.
 * @param Directory Otomatik dizin içeriğinin bulunduğu dizin.
 * @param host Ana bilgisayarın adresi.
 * @return Oluşturulan otomatik dizin içeriğindeki bağlantı.
 */

std::string Response::createDirectoryLink(std::string const& dirEntry, std::string Directory, std::string const& host)
{
    std::stringstream ss;
    
    // ".." veya "." değilse bağlantıyı oluştur
    if (dirEntry != ".." && dirEntry != ".")
    {
        ss << "\t\t<p><a href=\"http://" + host << Directory + "/" + dirEntry + "\">" + dirEntry + "/" + "</a></p>\n";
    }
    
    return ss.str();
}

/**
 * @brief Hata sayfasını oluşturur ve `_http`'ye ekler.
 */
void Response::errorPage()
{
    std::string line;
	std::ifstream fahd;
	std::string path;
	char *tmp_path;
	if (mp[toStringFor(_code)] == "")
	{
		tmp_path = realpath(".", NULL);
		path = toStringFor(tmp_path) + "/sources/Http(Errors)/" + toStringFor(_code) + ".html";
		free(tmp_path);
	}
	else 
	{
		tmp_path = realpath(".", NULL);
		path = toStringFor(tmp_path) + mp[toStringFor(_code)];
		free(tmp_path);
	}
	std::ifstream document;
	if ((access(path.c_str(), F_OK) == 0))
	{
		document.open(path, std::ios::in);
		while (getline(document, line))
		{
			_http += line + " ";
		}
	}
}

/**
 * HTTP isteğinin GET yöntemini işler.
 * Bu fonksiyon, GET isteğini işler ve istenen dosyanın varlığını kontrol eder.
 * İstek URL'sinden dosya yolunu elde eder, içerik türünü belirler ve varlık durumuna göre hata sayfası oluşturur.
 * İçerik uzunluğu, izin verilen maksimum boyutu aşarsa yanıt kodunu 413 (İstek Varlık Çok Büyük) olarak ayarlar.
 * Yanıt kodu bir sunucu hatası (500) belirtiyorsa hata sayfası oluşturur.
 * Son olarak, uygun durum kodu, sunucu adı, değiştirilme zamanı, içerik türü, içerik uzunluğu ve konumu içeren yanıt başlığını oluşturur
 * ve HTTP yanıt gövdesine ekler.
 *
 * @return GET yönteminin başarılı bir şekilde işlendiğini belirten 0 değerini döndürür.
 */
void Response::getContentType(std::string path)
{
	std::string imageExtensionsArray[] = { "jpeg", "jpg", "pjp", "jfif", "pjpeg" };
	std::set<std::string> imageExten_postValuesions(imageExtensionsArray, imageExtensionsArray + 5);
	_contentType = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
	_cgiType = _contentType;
	if (_contentType == "html")
		_contentType = "text/html";
	else if (_contentType == "pdf")
		_contentType = "application/pdf";
	else if (_contentType == "xml")
		_contentType = "text/xml";
	else if (_contentType == "js")
		_contentType = "text/javascript";
	else if (_contentType == "css")
		_contentType = "text/css";
	else if (imageExten_postValuesions.count(_contentType) > 0)
		_contentType = "image/jpeg";
	else if (_contentType == "png")
		_contentType= "image/png";
	else if (_contentType == "mp4")
		_contentType = "video/mp4";
	else if (_contentType == "webm")
		_contentType = "video/webm";
	else if (_contentType == "mpeg")
		_contentType = "video/mpeg";
	else if (_contentType == "mp3")
		_contentType = "audio/mpeg";
	else if (_contentType == "gif")
		_contentType = "image/gif";
	else if (_contentType == "ico")
		_contentType = "image/x-icon";
	else if (_contentType == "cpp")
		_contentType = "text/plain";
	else
		_contentType = "text/html";
}

/**
 * @brief böyle bir path var mı diye kontrol eder true false döndürür.
 * 
 * @param path 
 * @return true 
 * @return false 
 */
bool Response::checkIfPathIsFile(const char *path)
{
	struct stat s;
	if (stat(path, &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return false; // it's a director
		else if (s.st_mode & S_IFREG)
			return true; // it's a file
		else
			return true;
	}
	return true;
}

/**
 * @brief Dosyanın son değiştirilme tarihini kontrol eder ve kaydeder.
 *
 * Bu fonksiyon, `_path` üye değişkenine gösterilen dosyanın son değiştirilme tarihini kontrol eder.
 * Dosyanın durum bilgilerini `stat` fonksiyonu kullanarak elde eder.
 * Ardından, dosyanın son değiştirilme zamanını `_modifyTime` üye değişkenine kaydeder.
 * `_modifyTime` üyesi, dosyanın son değiştirilme tarihini RFC 1123 formatında ("Wdy, DD Mon 'YYYY' 'HH':'MM':'SS' 'GMT'") bir string olarak tutar.
 *
 * Bu fonksiyon, HTTP yanıtlarında `Last-Modified` başlığının ayarlanması için kullanılabilir.
 * İstemcilere, dosyanın son değiştirilme zamanını bildirmek amacıyla bu başlık kullanılır.
 */
void Response::checkModifyDate(void)
{
	char src[100];
	struct stat status;
	struct tm *That;

	if (stat(_path.c_str(), &status) == 0)
	{
		That = gmtime(&status.st_mtime);
		strftime(src, 100, "%a, %d %b %Y %H:%M:%S GMT", That);
		_modifyTime = std::string(src);
	}
}

/**
 * @brief Yanıtın tarihini ayarlar.
 *
 * Bu fonksiyon, `_time` üye değişkenine yanıtın tarihini ayarlar.
 * `gettimeofday` fonksiyonunu kullanarak geçerli zaman bilgisini elde eder.
 * Ardından, bu zaman bilgisini `_time` üyesine RFC 1123 formatında ("Wdy, DD Mon 'YYYY' 'HH':'MM':'SS' 'GMT'") bir string olarak kaydeder.
 *
 * `_time` üyesi, yanıtın `Date` başlığında kullanılmak üzere tarih bilgisini tutar.
 * Bu tarih, istemciye sunucunun yanıtı ne zaman oluşturduğunu bildirmek için kullanılır.
 */
void Response::setDate(void)
{
	struct tm *time;
	char stock[100];
	struct timeval timeval;

	gettimeofday(&timeval, NULL);
	time = gmtime(&timeval.tv_sec);
	strftime(stock, 100, "%a, %d %b %Y %H:%M:%S GMT", time);
	_time = std::string(stock);
}

/**
 * @brief Request'den gelen değerleri atadığımız yer
 * 
 */
void	Response::setupRequest()
{
    _host = _req.getFullHost();
    _protocol = _req.getProtocol();
    _type = _req.getMethod();
    _path = _req.getLocation();
	_fileName = _req.getFileName();
    _contentLen = _req.getContentLength();
    _buffit = _req.getRequestStr();

	_postmethod = false;
	_isUpload = false;
	_responseHeader = "";
	_hasRedirection = false;
}

/**
 * @brief POST methodundan gelen key value çiftini almak için kullanılır.
 * Örnek olarak: bir form POST'u geldiğini düşünürsek name alanı key'dir
 * girdi olarak verilen enes ise value değeridir.
 * @param query_string gelen request
 */
void Response::parseQueryString(const std::string &query_string)
{
	std::size_t position = 0;
	std::size_t i = 1;
	while (position < query_string.size())
	{
		const std::size_t next_delimiter = query_string.find('&', position);
		std::string query;
		if (next_delimiter == std::string::npos)
		{
			query = query_string.substr(position);
		}
		else
		{
			query = query_string.substr(position, next_delimiter - position);
		}
		const std::size_t pair_delimiter = query.find('=');
		const std::string name = query.substr(0, pair_delimiter);
		if (name.empty())
		{
			return;
		}
		std::string value;
		if (pair_delimiter != std::string::npos)
		{
			value = query.substr(pair_delimiter + 1);
		}
		_postValues.push_back(name + "=" + value);
		
		if (next_delimiter == std::string::npos)
		{
			break;
		}
		position = next_delimiter + 1;
		i++;
	}
	_envj = i; // kaç tane key value var tutulur
}

/**
 * @brief Yüklenen dosyanın işlenmesi ve kaydedilmesini gerçekleştirir.
 *
 * @param sear Dosya verilerinin başlangıcını içeren bir std::string.
 * @param buffer İstek verilerini içeren bir std::string.
 * @return Kaydedilen dosyanın adını içeren bir std::string.
 *
 * Bu fonksiyon, sear parametresi aracılığıyla dosya verilerinin başlangıcını alır ve buffer parametresi
 * aracılığıyla istek verilerini alır. Fonksiyon, bu verileri işleyerek dosyayı kaydeder ve kaydedilen dosyanın adını döndürür.
 * Dosya adı, yüklenen dosyanın adını içeren bir std::string olarak geri döner.
 * Fonksiyon, yüklenen dosyanın kaydedilmesi sırasında hatalar oluşması durumunda boş bir std::string döndürür.
 *
 * Dosya adı, _upload üye değişkenine göre belirlenen geçici bir dosya yoluna kaydedilir.
 * Dosya verileri sear parametresinden alınarak dosya içerisine yazılır ve dosya kapatılır.
 * Son olarak, kaydedilen dosyanın adı olan mainbuffer std::string olarak döndürülür.
 */
std::string Response::uploadFile(std::string sear, std::string buffer)
{
	std::string firstSixCharacters;
	std::string extension;
	std::string filename;
	std::string mainbuffer(buffer);
	size_t i = mainbuffer.rfind("filename=\"");
    if (i != std::string::npos) 
	{
        i += 10;
        size_t j = mainbuffer.find(".", i);  // "." karakterini ara
        if (j != std::string::npos) {
            filename = mainbuffer.substr(i, j - i);  // Dosya adını al

            // Şimdi dosya uzantısını al
            size_t k = mainbuffer.find("\"", j);  // Uzantının sonunu bul
            if (k != std::string::npos) {
                extension = mainbuffer.substr(j + 1, k - j - 1);  // Uzantıyı al
            }
        }
    }
	if (mainbuffer.length() > 10)
	{
		firstSixCharacters = filename.substr(0, 6);
		firstSixCharacters += "." + extension;
	}
	else
		firstSixCharacters = filename + "." + extension;

	std::string tmpFileName = "." + _upload;
	std::string root = "";
	int fd;
	if ("" == tmpFileName)
		fd = open((tmpFileName + "/" + firstSixCharacters).c_str(), O_RDWR | O_CREAT | O_TRUNC, 00777);
	else
	{
		mkdir((tmpFileName + "/" + "").c_str(), 0755);
		fd = open((tmpFileName + "/" + "" + "" + firstSixCharacters).c_str(), O_RDWR | O_CREAT | O_TRUNC, 00777);
	}
	if (fd == -1)
		return (std::string());
	write(fd, sear.c_str(), sear.size());
	close(fd);
	return (firstSixCharacters);
}

/**
 * HTTP isteğinin GET yöntemini işler.
 * Bu fonksiyon, GET isteğini işler ve istenen dosyanın varlığını kontrol eder.
 * İstek URL'sinden dosya yolunu elde eder, içerik türünü belirler ve varlık durumuna göre hata sayfası oluşturur.
 * İçerik uzunluğu, izin verilen maksimum boyutu aşarsa yanıt kodunu 413 (İstek Varlık Çok Büyük) olarak ayarlar.
 * Yanıt kodu bir sunucu hatası (500) belirtiyorsa hata sayfası oluşturur.
 * Son olarak, uygun durum kodu, sunucu adı, değiştirilme zamanı, içerik türü, içerik uzunluğu ve konumu içeren yanıt başlığını oluşturur
 * ve HTTP yanıt gövdesine ekler.
 *
 * @return GET yönteminin başarılı bir şekilde işlendiğini belirten 0 değerini döndürür.
 */
int Response::getMethodes()
{
	char* raw_path = realpath(".", NULL);
	std::string raw_path_str(raw_path);
	free(raw_path);

	std::string path = raw_path_str + removeAll(_path, raw_path_str);
	_path = path + "/";
	getContentType(path);
	fileExist(path.c_str());
	errorPage();
	if (_contentLen >= _maxBody)
	{
		_code = 413;
		_http = "";
		errorPage();
	}
	else
		_code = 200;
	if (_http.find("Status: 500") != std::string::npos)
	{
		_code = 500;
		_http = "";
		errorPage();
	}
	modifyResponseHeader();
	return 0;
}

void Response::modifyResponseHeader()
{
	_responseHeader += _protocol + " " + std::to_string(_code) + " " + _errorRep[_code];
	_responseHeader += "\nDate: " + _time;
	_responseHeader += "\nServer: " + _serverName;
	_responseHeader += "\nLast-modified: " + _modifyTime;
	_responseHeader += "\nContent-Type: " + _contentType;
	if (_isUpload != true)
		_responseHeader += "\nContent-Length: " + std::to_string(_http.size() - 1); // make a update for upload
	else
		_responseHeader += "\nContent-Length: " + std::to_string(_contentLen);
	_responseHeader += "\nContent-Location: " + _path;
	if(_postmethod != true)
		_responseHeader += "\nTransfer-Encoding: identity" + _encoding;
	_responseHeader += "\n\n";
	_responseHeader += _http;
}


/**
 * @brief DELETE isteğini işler ve yanıt başlığını oluşturur.
 *
 * DELETE isteğini gerçekleştirir ve sonuç olarak oluşan yanıt başlığını oluşturur.
 * İşlevleri şunlardır:
 *   - handleDeleteRequest() fonksiyonunu çağırarak DELETE isteğini gerçekleştirir.
 *   - _contentLen ve _maxBody değişkenlerini karşılaştırarak içeriğin uzunluğunu kontrol eder.
 *     Eğer aşılmışsa, _code değerini 413 olarak ayarlar.
 *   - _responseHeader değişkenine _protocol, _code ve _errorRep[_code] değerlerini ekler.
 *   - Date alanını _time değeriyle birlikte ekler.
 *   - resetHTML() fonksiyonunu çağırarak _http değişkenini sıfırlar.
 *   - errorPage() fonksiyonunu çağırarak hata sayfasını oluşturur.
 *   - Son olarak _http'yi _responseHeader'a ekler.
 *
 */
void Response::deleteMethodes()
{
	handleDeleteRequest();

	if (_contentLen >= _maxBody)
		_code = 413;
    
	_responseHeader += _protocol + " " + std::to_string(_code) + " " + _errorRep[_code];
	_responseHeader += "\nDate : " + _time + "\n\n";

	resetHTML();
	errorPage();

	_responseHeader += _http;
}

/**
 * @brief _http'nin değerlerini sıfırlar
 * 
 */
void Response::resetHTML()
{
    _http = "";
}

/**
 * @brief DELETE isteğini işleyerek belirtilen dosyayı veya dizini siler.
 * 
 * Bu fonksiyon, geçerli çalışma dizinine ve verilen dizine dayanarak mutlak yol oluşturur.
 * Yolun geçerli bir dosyaya işaret edip etmediğini kontrol eder. Dosya mevcutsa silinir.
 * İşlemin sonucuna bağlı olarak uygun HTTP durum kodu ayarlanır.
 */
void Response::handleDeleteRequest()
{
	char *tmp_path = realpath(".", NULL);
	std::string path = tmp_path + _path;
	free(tmp_path);
	if (checkIfPathIsFile(path.c_str()) == true)
	{
		if ((access(path.c_str(), F_OK) == 0))
		{
			if (remove(path.c_str()) == 0)
				_code = 204;
			else
				_code = 403;
		}
		else
			_code = 404;
	}
	else
		_code = 404;
}

std::string Response::getResponseHeader() { return _responseHeader; }
void	Response::setBando(std::string bando) { this->_bando = bando; }

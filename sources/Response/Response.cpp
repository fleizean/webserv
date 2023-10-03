#include "../../includes/Response.hpp"

Response::Response() {}

Response::Response(Request req, std::vector<ServerMembers*> servers, ServerMembers* matchedServer, std::string multiBody) : _req(req), _servers(servers), _matchedServer(matchedServer), _multiBody(multiBody)
{
    setupRequest();
}

Response::~Response() { }

template <typename T>
std::string toStringFor(T numb)
{
	std::stringstream stream;
	stream << numb;
	return stream.str();
}

/* --------> Functions <-------- */

void Response::run()
{
	std::string uri;
	std::string redirUriSlash;
	bool yes;

	for (std::vector<Location*>::iterator locIt = _matchedServer->getLocations().begin(); locIt != _matchedServer->getLocations().end(); ++locIt)
	{
		if (_path.find((*locIt)->getUri()) != std::string::npos)
		{
			_uriRoot = (*locIt)->getConfigMembers().getRoot();
			_cgiPath = _uriRoot;
			uri = (*locIt)->getUri();
			_maxBody = (*locIt)->getConfigMembers().getMaxClientBodySize();
			for (std::map<int, std::string>::iterator errIt = (*locIt)->getConfigMembers().getErrorPage().begin(); errIt != (*locIt)->getConfigMembers().getErrorPage().end(); ++errIt)
				mp.insert(std::make_pair(std::to_string(errIt->first), errIt->second));
			if ((*locIt)->getHasRedirection() == true) 
			{
				_hasRedirection = true;
				_redirectionType = (*locIt)->getRedirectionType();
				_redirectionURI = (*locIt)->getRedirectionURI();
			}
			for (std::vector<std::string>::iterator namesIt = (*locIt)->getConfigMembers().getAllowedMethods().begin(); namesIt != (*locIt)->getConfigMembers().getAllowedMethods().end(); ++namesIt)
				_allowedMethods.push_back(*namesIt);
 			yes = true;
		}
		else if (yes != true)
		{
			_allowedMethods.push_back("ALL");
			_maxBody = _matchedServer->getConfigMembers().getMaxClientBodySize();
		}
		for (std::map<std::string, std::string>::iterator namesIt = _matchedServer->getConfigMembers().getCgi().begin(); namesIt != _matchedServer->getConfigMembers().getCgi().end(); ++namesIt)
			cgi.insert(std::make_pair(namesIt->first, namesIt->second));
	}
	std::map<std::string, std::string> indexmap;
	for (std::vector<Location*>::iterator locIt = _matchedServer->getLocations().begin(); locIt != _matchedServer->getLocations().end(); ++locIt)
		for (std::vector<std::string>::iterator namesIt = (*locIt)->getConfigMembers().getIndex().begin(); namesIt != (*locIt)->getConfigMembers().getIndex().end(); ++namesIt)
			indexmap.insert(std::make_pair((*locIt)->getUri(), *namesIt));
	if (_hasRedirection == true) {

		for (std::vector<Location*>::iterator locIt = _matchedServer->getLocations().begin(); locIt != _matchedServer->getLocations().end(); ++locIt)
		{
			redirUriSlash = "/" + _redirectionURI;
			if ((*locIt)->getUri() == redirUriSlash)
			{
				if ((*locIt)->getUri() != "/") {
					_path = (*locIt)->getConfigMembers().getRoot();
					_maxBody = (*locIt)->getConfigMembers().getMaxClientBodySize();
					mp.clear();
					_allowedMethods.clear();
					for (std::map<int, std::string>::iterator errIt = (*locIt)->getConfigMembers().getErrorPage().begin(); errIt != (*locIt)->getConfigMembers().getErrorPage().end(); ++errIt)
						mp.insert(std::make_pair(std::to_string(errIt->first), errIt->second));
					for (std::vector<std::string>::iterator namesIt = (*locIt)->getConfigMembers().getAllowedMethods().begin(); namesIt != (*locIt)->getConfigMembers().getAllowedMethods().end(); ++namesIt)
						_allowedMethods.push_back(*namesIt);
				}
				_redirectionLocation = true;
				break;
			}
			else
			{
				_path = _uriRoot + _redirectionURI;
			}
		}
	}
	else
	{
		if (_path == uri) {
			_path += indexmap[uri];
		}
	}

	if (_redirectionLocation == true)
	{
		if (indexmap.find(redirUriSlash) != indexmap.end()) {
			_path += indexmap[redirUriSlash];
		}
	}

	size_t found = _path.find(uri);
	if (found != std::string::npos && _hasRedirection == false) {
		_path.replace(_path.find(uri), uri.length(), _uriRoot);
	}
	
	// isteği yönetmek için
	processRequest();
	
}

void Response::processRequest()
{
	if (_type == "GET" && (std::find(_allowedMethods.begin(), _allowedMethods.end(), "GET") != _allowedMethods.end() || std::find(_allowedMethods.begin(), _allowedMethods.end(), "ALL") != _allowedMethods.end()))
		getMethodes();
	else if (_type == "DELETE" && (std::find(_allowedMethods.begin(), _allowedMethods.end(), "DELETE") != _allowedMethods.end() || std::find(_allowedMethods.begin(), _allowedMethods.end(), "ALL") != _allowedMethods.end()))
		deleteMethodes();
	else if (_type == "POST" && (std::find(_allowedMethods.begin(), _allowedMethods.end(), "POST") != _allowedMethods.end() || std::find(_allowedMethods.begin(), _allowedMethods.end(), "ALL") != _allowedMethods.end())) 
		postMethodes();
	else
	{
		resetHTML();
		if (_type != "POST" || _type != "DELETE" || _type != "GET")
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
	_errorRep[200] = "OK";
	_errorRep[400] = "Bad Request";
	_errorRep[403] = "Forbidden";
	_errorRep[404] = "Not Found";
	_errorRep[405] = "Method Not Allowed";
	_errorRep[413] = "Payload Too Large";
	_errorRep[500] = "Internal Server Error";
	_errorRep[501] = "Not Implemented";
	_errorRep[502] = "Bad Gateway";
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
        autoindx[(*it)->getListen().port] = (*it)->getConfigMembers().getAutoIndex();
    
    std::ifstream document;
    // Belirtilen yol bir dosya mı yoksa klasör mi diye kontrol et
    if (checkIfPathIsFile(fileName) == true)
    {
        if ((access(fileName, F_OK) == 0)) // dosya var mı yok mu diye kontrol eder
        {
            document.open(fileName, std::ifstream::in);
            
            // Dosya açılabiliyorsa
            if (!document.is_open())
            {
                _code = 403;
                return _code;
            }
            
            std::stringstream test;
            test << document.rdbuf();
            _http = test.str();
        }
        else
            _code = 404;
        return _code;
    }
    else if (checkIfPathIsFile(fileName) == false && _type == "GET" && autoindx[atoi(_host.substr(_host.find(":") + 1).c_str())] == 1)
    {
        // Otomatik dizin listelemesi etkinse, HTTP yanıtına otomatik dizin içeriğini ekle
		if (fAutoIndex(fileName) == "ERROR")
		{
			resetHTML();
			_code = 403;
		}
		else
		{
			_http += fAutoIndex(fileName);
        	_code = 200;
        	
		}
        return _code;
    }
    else
        _code = 404;
    return _code;
}

/**
 * @brief Belirtilen dizindeki otomatik indeks sayfasını oluşturur.
 *
 * @param path İçeriği taranacak dizinin yolu
 * @function
 * @return Oluşturulan otomatik indeks sayfası HTML olarak döndürülür.
 *
 * Bu işlev, verilen `path` dizinindeki dosyaları ve dizinleri listeleyerek
 * otomatik indeks sayfasını oluşturur. Sayfa, HTML formatında bir dize olarak
 * döndürülür.
 */

std::string Response::fAutoIndex(const char* path)
{
    std::string directory(_path);
    DIR* dir = opendir(path);
    std::string autoIndexPage =
        "<!DOCTYPE html>\n\
        <html>\n\
        <head>\n\
                <title>" +
        directory + "</title>\n\
        </head>\n\
        <body>\n\
        <h1>Auto Index Page</h1>\n\
        <p>\n";
    
    if (dir == NULL)
    {
        std::cerr << "Error: could not open the following path " << path << std::endl;
        return "ERROR";
    }

	// eklenebilir
	if (!directory.empty() && directory.back() == '/')
	    directory.pop_back();
    
    // Dizin içeriğini tarayarak otomatik dizin içeriğini oluştur
    for (struct dirent* dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) // sürekli o anki alanımızdaki (klasör) dosyaları almamızı sağlayacak döngü
    {
		/* std::cout << RED << dirEntry->d_name << RESET << std::endl; */ // Evoda açılacak kod satırı
        autoIndexPage += createDirectoryLink(std::string(dirEntry->d_name), directory, _host); // d_name o anki işlenecek dosyanın adı
    }
    
	// Otomatik indeks sayfasının son kısmını oluşturur.
    autoIndexPage +=
        "\
        </p>\n\
        </body>\n\
        </html>\n";
    
    closedir(dir);
    return autoIndexPage;
}

/**
 * @brief Dizin girişi için otomatik dizin içeriğindeki bir bağlantı oluşturur. 
 * 
 * Bunu a href kullanarak yönlendirme yoluyla yapar bu şekilde klasörler arasında gezinebilir hale gelir.
 * 
 * @param dirEntry Dizin girişi.
 * @param Directory Otomatik dizin içeriğinin bulunduğu dizin.
 * @param host Ana bilgisayarın adresi.
 * @return Oluşturulan otomatik dizin içeriğindeki bağlantı.
 */

std::string Response::createDirectoryLink(std::string const& dirEntry, std::string directory, std::string const& host)
{
    std::stringstream ss;
    
    // ".." veya "." değilse bağlantıyı oluştur (sebebi güvenlik açıklarını engellemektir .. ve . kullanarak önceki dizinlere gitmesini istemiyorum)
    if (dirEntry != ".." && dirEntry != ".")
    {
        ss << "\t\t<p><a href=\"http://" + host << directory + "/" + dirEntry + "\">" + dirEntry + "/" + "</a></p>\n";
    }
    
    return ss.str();
}


bool Response::checkCgiForConfig()
{
	bool flag = false;
	std::map<std::string, std::string>::iterator it;
	for (it = cgi.begin(); it != cgi.end(); ++it)
	{
		if (it->first == ".py" && it->second == "/usr/bin/python3")
			return true;
		else
			flag = false;
	}
	return flag;
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
	_code = 200;
	std::string path = raw_path_str + removeSubstring(_path, raw_path_str); // o anki konum hali hazırda gelen path yolunda varsa kaldırır daha sonrasında ekleme yapacağı için
	_path = path + "/";
	getContentType(path);
	fileExist(path.c_str());
	if (_contentLen >= _maxBody || _http.length() >= (size_t)_maxBody)
	{
		_code = 413;
		resetHTML();
	}		
	errorPage();
	modifyResponseHeader();
	return 0;
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
	_postMethod = true;
	char* pointPath = realpath(".", NULL);
	std::string path = pointPath + removeSubstring(_path, pointPath);
	free(pointPath);
	_code = 200;
	parseQueryString(_requestHeader.substr(_requestHeader.find("\r\n\r\n") + strlen("\r\n\r\n")));
	getContentType(path);
	if (_path.substr(_path.find_last_of(".") + 1) == "py" && checkCgiForConfig()){
		Cgi _cgi(path.c_str(), _requestHeader, _req, "/usr/bin/python3", _postValues, _matchedServer, _cgiPath, _multiBody);
		_http = _cgi.cgiExecute();
		if (_cgi.getStatusCode() == 500) {
			resetHTML();
			_code = 500;
		}
	}
	else
		_code = 502;
	if (_contentLen >= _maxBody || _http.length() >= (size_t)_maxBody)
		_code = 413;
	errorPage();
	modifyResponseHeader();
	return 0;
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
 * @brief Yanıtın tarihini ayarlar.
 *
 * Bu fonksiyon, `_time` üye değişkenine yanıtın tarihini ayarlar.
 * `gettimeofday` fonksiyonunu kullanarak geçerli zaman bilgisini elde eder.
 * Ardından, bu zaman bilgisini `_time` üyesine RFC 1123 formatında ("Wdy, DD Mon 'YYYY' 'HH':'MM':'SS' 'GMT'") bir string olarak kaydeder.
 *
 * `_time` üyesi, yanıtın `Date` başlığında kullanılmak üzere tarih bilgisini tutar.
 * Bu tarih, istemciye sunucunun yanıtı ne zaman oluşturduğunu bildirmek için kullanılır.
 */
void Response::setDate()
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
	if (_multiBody != "")
		_body = _multiBody;
	else
		_body = _req.getBody();
	mp.clear();
	cgi.clear();
	_postMethod = false;
	_isUpload = false;
	_responseHeader = "";
	_hasRedirection = false;
	_redirectionLocation = false;
}

/**
 * @brief POST methodundan gelen key value çiftini almak için kullanılır.
 * Örnek olarak: bir form POST'u geldiğini düşünürsek name alanı key'dir
 * girdi olarak verilen enes ise value değeridir.
 * @param queryString gelen request
 */
void Response::parseQueryString(const std::string &queryString)
{
	std::istringstream iss(queryString);
    std::string line;

    while (std::getline(iss, line))
    {
        _postValues.push_back(line);
    }
}

void Response::modifyResponseHeader()
{
	if (_hasRedirection == true)
		_code = _redirectionType;
	_responseHeader += _protocol + " " + std::to_string(_code) + " " + _errorRep[_code];
	_responseHeader += "\nDate: " + _time;
	_responseHeader += "\nConnection: close";
	if (_matchedServer->getServerHeader().empty())
		_responseHeader += "\nServer: webserv";
	else
		_responseHeader += "\nServer: " + _matchedServer->getServerHeader();
	_responseHeader += "\nContent-Type: " + _contentType;
	if (_isUpload != true)
		_responseHeader += "\nContent-Length: " + std::to_string(_http.size() - 1); // make a update for upload
	else
		_responseHeader += "\nContent-Length: " + std::to_string(_contentLen);
	_responseHeader += "\nContent-Location: " + _path.substr(1);
	if(_postMethod != true)
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
				_code = 200;
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
void	Response::setBuffer(std::string requestHeader) { this->_requestHeader = requestHeader; }

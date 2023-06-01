#include "../../includes/Response.hpp"

Response::Response(Request req, std::vector<ServerMembers*> servers) : _req(req), _servers(servers)
{
    setupRequest();
}

Response::~Response() { }

/* --------> Functions <-------- */

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
            /* if (_cgiType == "py" && mp[".py"].find("/usr/bin/python") != std::string::npos)
            {
                _http = g.cgiExecute("/usr/bin/python", fileName, _postName, _postValue, 0, _bando, _code);
                _code = 200;
                return _code;
            }
            else if (_cgiType == "pl" && mp[".pl"].find("/usr/bin/perl") != std::string::npos)
            {
                _http = g.cgiExecute("/usr/bin/perl", fileName, _postName, _postValue, 0, _bando, _code);
                _code = 200;
                return _code;
            }
            else if (_cgiType == "php" && mp[".php"].find("/usr/bin/php-cgi") != std::string::npos)
            {
                _http = g.cgiExecute("/usr/bin/php-cgi", fileName, _postName, _postValue, 0, _bando, _code);
                _code = 200;
                return _code;
            } */
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
 * @brief Hata sayfasını oluşturur ve `MainServer.http`'ye ekler.
 */
void Response::errorPage()
{
    std::string line;
    std::ifstream fahd;
    std::string path;
    char buf[PATH_MAX];
    
    // Hata koduna karşılık gelen dosyanın yolunu belirle
    if (mp[std::to_string(_code)] == "")
    {
        if (realpath(".", buf) != NULL)
            path = std::string(buf) + "/srcs/Http(Errors)/" + std::to_string(_code) + ".html";
    }
    else
    {
         if (realpath(".", buf) != NULL)
            path = std::string(buf) + std::to_string(_code) + ".html";
    }
    
    std::ifstream document;
    
    // Dosyanın var olup olmadığını kontrol et
    if ((access(path.c_str(), F_OK) == 0))
    {
        document.open(path, std::ios::in);
        
        // Dosyayı satır satır oku ve MainServer.http'ye ekle
        while (getline(document, line))
        {
            _http += line + " ";
        }
    }
}

void Response::getContentType(std::string path)
{
    std::set<std::string> imageExtensions = { "jpeg", "jpg", "pjp", "jfif", "pjpeg" };
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
	else if (imageExtensions.count(_contentType) > 0)
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
	else if (_contentType == "doc")
		_contentType = "application/msword";
	else if (_contentType == "gif")
		_contentType = "image/gif";
	else if (_contentType == "ico")
		_contentType = "image/x-icon";
	else if (_contentType == "cpp")
		_contentType = "text/plain";
	else
		_contentType = "text/html";
}

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
    _host = _req.getHost();
    _protocol = _req.getProtocol();
    _type = _req.getMethod();
    _path = _req.getLocation();
    _contentLen = _req.getContentLength();
    _buffit = _req.getRequestStr();
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
		_postName[i] = name;
		_postValue[i] = value;

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
    std::string mainbuffer(buffer);
    size_t i = mainbuffer.rfind("filename=\"");
    if (i != std::string::npos)
    {
        i += 10;
        size_t j = mainbuffer.find("\"", i);
        if (j != std::string::npos)
        {
            mainbuffer = mainbuffer.substr(i, j - i);
        }
    }
    std::string tmpFile = "." + _upload;
    std::string filePath = tmpFile + "/" + tmpFile;
    int fd = open(filePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 00777);
    if (fd == -1)
        return std::string();

    write(fd, sear.c_str(), sear.size());
    close(fd);
    return mainbuffer;
}
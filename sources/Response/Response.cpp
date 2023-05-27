#include "../../includes/Response.hpp"

Response::Response() {}

Response::~Response() {}

void Response::parse_buf(char *buf, std::string &filename, std::string &content_type)
{
	std::stringstream ss(buf);
	std::string word;

	while (ss >> word)
	{
		if (word.size() > 9 && word.substr(0, 9) == "filename=")
			filename = word.substr(9);
		if (word.size() == 13 && word.substr(0, 13) == "Content-Type:"){
			ss >> content_type;
			std::cout << "content_type: " << content_type << std::endl;
		}
	}
}


void parse_post(char *buf, std::string &filename)
{
	std::stringstream ss(buf);
	std::string word;

	while (ss >> word)
	{
		if (word.size() > 9 && word.substr(0, 9) == "filename=")
			filename = word.substr(9);
	}
}

std::string removeAll(std::string str, const std::string &from)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from)) != std::string::npos)
	{
		str.erase(start_pos, from.length());
	}
	return str;
}

bool Response::CheckifPathisFile(const char *path)
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

void Response::setDate(void)
{
	struct tm *Time;
	char stock[100];
	struct timeval timeval;

	gettimeofday(&timeval, NULL);
	Time = gmtime(&timeval.tv_sec);
	strftime(stock, 100, "%a, %d %b %Y %H:%M:%S GMT", Time);
	time = std::string(stock);
}

void Response::CheckModiDate(void)
{
	char src[100];
	struct stat status;
	struct tm *That;

	if (stat(path.c_str(), &status) == 0)
	{
		That = gmtime(&status.st_mtime);
		strftime(src, 100, "%a, %d %b %Y %H:%M:%S GMT", That);
		Modiftime = std::string(src);
	}
}

void Response::GetContent_Type(std::string path)
{
	content_type = path.substr(path.rfind(".") + 1, path.size() - path.rfind("."));
	cgitype = content_type;
	if (content_type == "html")
		content_type = "text/html";
	else if (content_type == "pdf")
		content_type = "application/pdf";
	else if (content_type == "xml")
		content_type = "text/xml";
	else if (content_type == "js")
		content_type = "text/javascript";
	else if (content_type == "css")
		content_type = "text/css";
	else if (content_type == "jpeg" || content_type == "jpg" || content_type == "pjp" || content_type == "jfif" || content_type == "pjpeg")
		content_type = "image/jpeg";
	else if (content_type == "png")
		content_type = "image/png";
	else if (content_type == "mp4")
		content_type = "video/mp4";
	else if (content_type == "webm")
		content_type = "video/webm";
	else if (content_type == "mpeg")
		content_type = "video/mpeg";
	else if (content_type == "mp3")
		content_type = "audio/mpeg";
	else if (content_type == "doc")
		content_type = "application/msword";
	else if (content_type == "gif")
		content_type = "image/gif";
	else if (content_type == "ico")
		content_type = "image/x-icon";
	else if (content_type == ".cpp")
		content_type = "text/plain";
	else
		content_type = "text/html";
}
template <typename T>
std::string ToString(T numb)
{
	std::stringstream stream;
	stream << numb;
	return stream.str();
}

void Response::Methodes(std::vector<ServerMembers*> _server)
{
	this->srvr = _server;
	std::vector<ServerMembers*>::iterator it = srvr.begin();
	std::vector<ServerMembers*>::iterator ss = srvr.begin();
	std::vector<ServerMembers*>::iterator xx = srvr.end();
	int j = 1;
	int jn = 1;
	std::string aled;
	std::string all;
	bool yes;
	std::map<int, int> portfind;
	for (int pl = 1; ss != xx; ++ss, ++pl, jn++)
		portfind.insert(std::make_pair((*ss)->getListen().port, jn));
	std::vector<Location *>::iterator locItos = (*it)->getLocations().begin();
	for (int i = 1; i != portfind[atoi(host.substr(host.find(":") + 1).c_str())]; i++)
		it++;
	for (std::vector<Location*>::iterator locIt = (*it)->getLocations().begin(); locIt != (*it)->getLocations().end(); ++locIt, locItos++, ++j)
	{
		if (path.find((*locIt)->getUri()) != std::string::npos)
		{
			uril_root = (*locIt)->getConfigMembers().getRoot();
			aled = (*locIt)->getUri();
			max_body = (*locIt)->getConfigMembers().getMaxClientBodySize();
			for (std::map<int, std::string>::iterator errIt = (*locIt)->getConfigMembers().getErrorPage().begin(); errIt != (*locIt)->getConfigMembers().getErrorPage().end(); ++errIt)
				mp.insert(std::make_pair(ToString(errIt->first), errIt->second));
			for (std::vector<std::string>::iterator namesIt = (*locItos)->getConfigMembers().getAllowedMethods().begin(); namesIt != (*locItos)->getConfigMembers().getAllowedMethods().end(); ++namesIt)
				all += *namesIt;
			yes = true;
		}
		else if (yes != true)
		{
			all = "GETPOSTDELETE";
			max_body = (*it)->getConfigMembers().getMaxClientBodySize();
		}
		for (std::map<std::string, std::string>::iterator namesIt = (*it)->getConfigMembers().getCgi().begin(); namesIt != (*it)->getConfigMembers().getCgi().begin(); ++namesIt)
			mp.insert(std::make_pair(namesIt->first, namesIt->second));
	}
	for (std::vector<std::string>::const_iterator namesIt = (*it)->getServerName().begin(); namesIt != (*it)->getServerName().end(); ++namesIt)
		serv_name = *namesIt;
	std::map<std::string, std::string> indexmap;
	for (std::vector<Location*>::iterator locIt = (*it)->getLocations().begin(); locIt != (*it)->getLocations().end(); ++locIt, ++j)
		for (std::vector<std::string>::iterator namesIt = (*locIt)->getConfigMembers().getIndex().begin(); namesIt != (*locIt)->getConfigMembers().getIndex().end(); ++namesIt)
			indexmap.insert(std::make_pair((*locIt)->getUri(), *namesIt));
	if (path == aled)
		path += indexmap[aled];
	path.replace(path.find(aled), aled.length(), uril_root);
	;
	TheReposn = "";
	if (type == "GET" && all.find(type) != std::string::npos)
		GetMethodes();
	else if (type == "DELETE" && all.find(type) != std::string::npos)
		DeleteMethodes();
	else if (type == "POST" && all.find(type) != std::string::npos)
		POSTMethodes();
	else
	{
		reseat();
		if (type == "POST" || type == "DELETE" || type == "GET")
			code = 405;
		else
			code = 501;
		ErrorPage();
		TheReposn += protocol + " " + ToString(code) + " " + EroRep[code];
		TheReposn += "\nDate : " + time;
		TheReposn += "\nServer: Webserv /1.0.0";
		TheReposn += "\n\n";
		TheReposn += http;
	}
}

int Response::filexist(const char *fileName)
{
	std::string line;
	code = 200;
	
	int autoindx[65535];
	std::vector<ServerMembers*>::iterator ite = srvr.end();
	std::vector<ServerMembers*>::iterator it = srvr.begin();
	for (int i = 1; it != ite; ++it, ++i)
	{
		autoindx[(*it)->getListen().port] = (*it)->getConfigMembers().getAutoIndex();
	}
	std::ifstream document;
	if (CheckifPathisFile(fileName) == true)
	{
		if ((access(fileName, F_OK) == 0))
		{
			document.open(fileName, std::ifstream::in);
			if (document.is_open() == false)
			{
				code = 403;
				return code;
			}
			std::stringstream test;
			test << document.rdbuf();
			http = test.str();
			if (cgitype == "py" && mp[".py"].find("/usr/bin/python") != std::string::npos)
			{
				http = g.cgiExecute("/usr/bin/python", fileName, postname, postvalue, 0, bando);
				code = 200;
				return code;
			}
			else if (cgitype == "pl" && mp[".pl"].find("/usr/bin/perl") != std::string::npos)
			{
				http = g.cgiExecute("/usr/bin/perl", fileName, postname, postvalue, 0, bando);
				code = 200;
				return code;
			}
			else if (cgitype == "php" && mp[".php"].find("/usr/bin/php-cgi") != std::string::npos)
			{
				http = g.cgiExecute("/usr/bin/php-cgi", fileName, postname, postvalue, 0, bando);
				code = 200;
				return code;
			}
			return code;
		}
		else
			code = 404;
		return code;
	}
	else if (CheckifPathisFile(fileName) == false && type == "GET" && autoindx[atoi(host.substr(host.find(":") + 1).c_str())] == 1)
	{
		http += FautoIndex(fileName);
		code = 200;
		return code;
	}
	else
		code = 404;
	return code;
}
void Response::ErrorPage(void)
{
	std::string line;
	std::ifstream fahd;
	std::string path;
	if (mp[ToString(code)] == "")
	{
		path = ToString(realpath(".", NULL)) + "/srcs/Http(Errors)/" + ToString(code) + ".html";
	}
	else
		path = ToString(realpath(".", NULL)) + mp[ToString(code)];
	std::ifstream document;
	if ((access(path.c_str(), F_OK) == 0))
	{
		document.open(path, std::ios::in);
		while (getline(document, line))
		{
			http += line + " ";
		}
	}
}

void Response::Erostatus(void)
{
	EroRep[100] = "Continue";
	EroRep[200] = "OK";
	EroRep[201] = "Created";
	EroRep[204] = "No Content";
	EroRep[400] = "Bad Request";
	EroRep[403] = "Forbidden";
	EroRep[404] = "Not Found";
	EroRep[405] = "Method Not Allowed";
	EroRep[413] = "Payload Too Large";
	EroRep[500] = "Internal Server Error";
	EroRep[501] = "Not Implemented";
}

int Response::GetMethodes(void)
{
	std::string raw_path = realpath(".", NULL);
	std::string p = realpath(".", NULL) + removeAll(path, realpath(".", NULL));
	if (p.find(raw_path + raw_path) != std::string::npos)
		p = p.substr(p.find(raw_path));
	path = p + "/";
	GetContent_Type(p);
	filexist(p.c_str());
	std::string testit;
	ErrorPage();
	if (content_len >= max_body)
	{
		code = 413;
		http = "";
		ErrorPage();
	}
	else
		code = 200;
	std::stringstream test;
	if (http.find("Status: 500") != std::string::npos)
	{
		code = 500;
		http = "";
		ErrorPage();
	}
	TheReposn += protocol + " " + ToString(code) + " " + EroRep[code];
	TheReposn += "\nDate : " + time;
	TheReposn += "\nServer: " + serv_name;
	TheReposn += "\nLast-modified " + Modiftime;
	TheReposn += "\nContent-Type: " + content_type;
	TheReposn += "\nContent-Length : " + std::to_string(http.size() - 1);
	TheReposn += "\nContent-Location: " + path;
	TheReposn += "\nTransfer-Encoding: identity" + encoding;
	TheReposn += "\n\n";
	TheReposn += http;
	return 0;
}

void Response::DeletePathHandler(void)
{
	std::string p = realpath(".", NULL) + path;
	if (CheckifPathisFile(p.c_str()) == true)
	{
		if ((access(p.c_str(), F_OK) == 0))
		{
			if (remove(p.c_str()) == 0)
				code = 204;
			else
				code = 403;
		}
		else
			code = 404;
	}
	else
		code = 404;
}

void Response::DeleteMethodes(void)
{
	DeletePathHandler();
	if (content_len >= max_body)
		code = 413;
	TheReposn += protocol + " " + ToString(code) + " " + EroRep[code];
	TheReposn += "\nDate : " + time;
	TheReposn += "\n\n";
	reseat();
	ErrorPage();
	TheReposn += http;
}

std::string get_right_of_delim(std::string const &str, std::string const &delim)
{
	return str.substr(str.find(delim) + delim.size());
}

void Response::ParseQueryString_(const std::string &query_string)
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
		postname[i] = name;
		postvalue[i] = value;
		if (next_delimiter == std::string::npos)
		{
			break;
		}
		position = next_delimiter + 1;
		i++;
	}
	envj = i;
}

int Response::POSTMethodes()
{
	std::string p = realpath(".", NULL) + removeAll(path, realpath(".", NULL));
	code = 413;
	ParseQueryString_(bando.substr(bando.find("\r\n\r\n") + strlen("\r\n\r\n")));
	GetContent_Type(p);
	if (path.substr(path.find_last_of(".") + 1) == "php")
		http = g.cgiExecute("/usr/bin/php", p.c_str(), postname, postvalue, 0, bando);
	if (path.substr(path.find_last_of(".") + 1) == "py")
		http = g.cgiExecute("/usr/bin/python", p.c_str(), postname, postvalue, envj, bando);
	bool upload = false;
	if (content_len <= max_body)
	{
		code = 200;
		std::string sear(bando);
		size_t i = sear.rfind("Content-Type:");
		if (i != std::string::npos)
			i = sear.find("\n", i);
		if (i != std::string::npos)
		{
			size_t j = sear.find("------WebKitFormBoundary", i);
			if (j != std::string::npos)
			{
				upload = true;
				http = "<!DOCTYPE htm1l>\n<html>\n<h1>File " + filename + "Hs Been Uploaded</h1>";
				http = "<a href=" + removeAll(filename, "\"") + " download>Download File</a>";
			}
		}
	}
	if (content_len >= max_body)
		code = 413;
	if (http.find("Status: 500") != std::string::npos)
	{
		code = 500;
		http = "";
		ErrorPage();
	}
	TheReposn += protocol + " " + ToString(code) + " " + EroRep[code];
	TheReposn += "\nDate : " + time;
	TheReposn += "\nServer: " + serv_name;
	TheReposn += "\nContent-Type: " + content_type;
	if (upload != true)
		TheReposn += "\nContent-Length : " + std::to_string(http.size() - 1); // make a update for upload
	else
		TheReposn += "\nContent-Length : " + ToString(content_len);
	TheReposn += "\nContent-Location: " + path;
	TheReposn += "\n\n";
	ErrorPage();
	TheReposn += http;
	return 0;
}

void Response::reseat(void)
{
	http = "";
}

std::string Response::geit(std::string const &dirEntry, std::string Directory, std::string const &host)
{
	std::stringstream ss;
	if (dirEntry != ".." && dirEntry != ".")
		ss << "\t\t<p><a href=\"http://" + host << Directory + "/" + dirEntry + "\">" + dirEntry + "/" + "</a></p>\n";
	return ss.str();
}

std::string Response::FautoIndex(const char *path)
{
	std::string Directory(path);
	DIR *dir = opendir(path);
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
	for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		Autoindex_Page += geit(std::string(dirEntry->d_name), Directory, host);
	}
	Autoindex_Page += "\
    </p>\n\
    </body>\n\
    </html>\n";
	closedir(dir);
	return Autoindex_Page;
}
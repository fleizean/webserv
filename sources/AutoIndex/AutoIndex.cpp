#include "../../includes/AutoIndex.hpp"

std::string	AutoIndex::renderPage(std::string directory, std::string path, std::string host) {
	DIR *dir;
	struct dirent *ent;
	std::string page;
	std::string port = &host[host.find(":") + 1];
	host.erase(host.find(":"));
	page.insert(page.size(), "<html>\n<head><title>Index of ");
	page.insert(page.size(), host);
	page.insert(page.size(), "</title></head>\n<body bgcolor=\"white\">\n<h1>Index of ");
	page.insert(page.size(), host);
	page.insert(page.size(), "</h1><body><pre><hr>\n");
	directory = path.substr(path.find_last_of("/") + 1);
	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (strcmp(".", ent->d_name) != 0 && strcmp("..", ent->d_name) != 0) {
				page.insert(page.size(), "<a href=\"http://");
				page.insert(page.size(), host);
				if (strcmp(port.c_str(), "0") != 0) {
					page.insert(page.size(), ":");
					page.insert(page.size(), port.c_str());
				}
				if (directory != "") {
					page.insert(page.size(), "/");
					page.insert(page.size(), directory);
				}
				page.insert(page.size(), "/");
				page.insert(page.size(), ent->d_name);
				page.insert(page.size(), "\">");
				page.insert(page.size(), ent->d_name);
				page.insert(page.size(), "</a>\n");
			}
		}
		closedir (dir);
	} 
	else {
		perror ("Autoindex");
		page.clear();
		return "";
	}
	page.insert(page.size(), "</pre><hr></body>");
	page.insert(page.size(), "</html>\n");
	return page;
}
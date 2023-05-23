#pragma once

#include "Include.hpp"
#include "ServerMembers.hpp"
#include "Location.hpp"
#include "ConfigMembers.hpp"

class Config
{
private:
	string _configContent;
	string _tmpLocationUri;

	/* for control */
	bool serverBlock;
	bool mainBlock;
	bool locationBlock;

	std::vector<ServerMembers*> _parsedServers;

	/*std::map<long, ServerMembers> _servers;
	std::map<long, ServerMembers *>	_sockets;*/

public:
	/* default constructor */
	Config();
	/* copy constructor */
	Config(Config const &rhs);
	/* destructor */
	~Config();
	/* operator */
	Config &operator=(Config const &rhs);
	std::vector<ServerMembers*>& getConfig(void);
	
	/* methods */
	void FileChecker(const string &conf_path);
	void split_server(std::string _configContent);
	void parse_server();

	/* First Area */
	void endScopeConf();

	/* Main Blocks for Parse */
	void parseMainArea(std::string& line);
	void parseServerArea(std::string& line);
	void parseLocationArea(std::string& line);

	/* ConfigMembers Parse */
	void parseRoot(std::stringstream& ss, ConfigMembers &cm);
	void parseMaxClientBodySize(std::stringstream& ss, ConfigMembers& cm);
	void parseErrorPage(std::stringstream& ss, ConfigMembers& cm);
	void parseIndex(std::stringstream& ss, ConfigMembers& cm);
	void parseAutoIndex(std::stringstream& ss, ConfigMembers& cm);
	void parseCgi(std::stringstream& ss, ConfigMembers &cm);
	void parseAllowedMethods(std::stringstream& ss, ConfigMembers &cm);

	/* Server Parse */
	void parseListen(std::stringstream& ss, ServerMembers &srvr);
	void parseServerName(std::stringstream& ss, ServerMembers &srvr);
	void parseLocation(std::stringstream& ss, ServerMembers &srvr);

	/* Location Parse */
	
	void parseReturn(std::stringstream& ss, Location &lctn);

	/* Print All */
	void printAll();
};

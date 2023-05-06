#pragma once

#include "include.hpp"

class ConfigMembers
{
private:
	std::string                         _root;
	size_t                              _maxClientBodySize;
	std::map<int, std::string>          _errorPage;
	bool                                _autoIndex;
	std::vector<std::string>            _index;
	std::map<std::string, std::string>  _cgis;
public:
	ConfigMembers();
	ConfigMembers(ConfigMembers const &rhs);
	ConfigMembers &operator=(ConfigMembers const &rhs);
	~ConfigMembers();

	/* get Area */
	std::string const &getRoot() const;
	size_t const &getMaxClientBodySize() const;
	std::map<int, std::string>& getErrorPage();
	bool const &getAutoIndex() const;
	std::vector<std::string>& getIndex();
	std::map<std::string, std::string>& getCgis();
	
	/* set Area */
	void setRoot(std::string const &root);
	void setMaxClientBodySize(size_t const &maxClientBodySize);
	void setAutoIndex(bool const &autoIndex);
};
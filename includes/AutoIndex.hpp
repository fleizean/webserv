#pragma once

#include "Include.hpp"

class AutoIndex {
	public:
		AutoIndex(){};
		std::string	renderPage(std::string directory, std::string path, std::string host);
		~AutoIndex(){};	
};

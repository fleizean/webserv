#pragma once

#include <signal.h>
#include <iostream>
#include <fstream>

class Webserv
{
public:
    Webserv(const std::string conf_file = "");
    ~Webserv();
};
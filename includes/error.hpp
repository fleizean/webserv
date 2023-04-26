#pragma once

#include <string>
#include <map>
#include <iostream>
#include "../includes/color.hpp"

class Error
{
private:
    Error();
    Error(Error const &rhs);
    Error &operator=(Error const &rhs);

    std::map<int, std::string> error_list;
    int err_code;
public:
    Error(const int &err_code);
    ~Error();

    void setAndPrint(const int &err_code);
};
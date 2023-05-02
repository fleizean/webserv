#pragma once

#include "include.hpp"

class Error
{
private:
    /* default constructor */
    Error();
    /* default constructor */
    Error(Error const &rhs);
    Error &operator=(Error const &rhs);

    std::map<int, std::string> error_list;
    int err_code;
public:
    Error(const int &err_code);
    /* destructor */
    ~Error();
    /* methods */
    void setAndPrint(const int &err_code);
};
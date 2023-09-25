#pragma once

#include "Include.hpp"

class Error
{
private:
    /* default constructor */
    
    /* default constructor */
    Error(Error const &rhs);
    Error &operator=(Error const &rhs);

    std::map<int, std::string> error_list;
    int err_code;
public:
    Error();
    Error(const int &err_code);
    /* destructor */
    ~Error();
    /* methods */
    void setAndPrint(const int &err_code, std::string const &line);
};

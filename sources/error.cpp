#include "../includes/error.hpp"

Error::Error(const int& err_code) : err_code(err_code)
{
    error_list[0] = "NULL";
    error_list[1] = "Config file argument not found.";
    error_list[2] = "Could not open config file.";
    error_list[3] = "Invalid argument.";
}

Error::~Error() { }

void Error::setAndPrint(const int &err_code)
{
    this->err_code = err_code;
    for(std::map<int, std::string>::const_iterator it = error_list.begin(); it != error_list.end(); ++it)
    {
        if (it->first == this->err_code){
            std::cerr << BOLD_RED << "Error " << it->first << ": " << it->second << RESET << std::endl;
            return;
        }
    }
    std::cerr << "Unkown error code." << std::endl;
}

#include "../includes/Error.hpp"

Error::Error(const int& err_code) : err_code(err_code)
{
    error_list[0] = "NULL";
    error_list[1] = "Config file argument not found.";
    error_list[2] = "Could not open config file."; 
    error_list[3] = "Config file is empty";
    error_list[4] = "Invalid file extension.";
    error_list[5] = "Invalid argument.";
    error_list[6] = "Please provide a config file.";
    error_list[7] = "Parenthesis is unbalanced.";
    error_list[8] = "Expected 'server {'";
    error_list[9] = "Expected 'location'";
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
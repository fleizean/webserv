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
    error_list[9] = "Shouldn't be 'location'";
    error_list[10] = "Location must be end with ';'";
    error_list[11] = "Expected listen argument";
    error_list[12] = "Expected server argument";
    error_list[13] = "Expected root argument";
    error_list[14] = "Root requires only 1 argument";
    error_list[15] = "Expected index argument";
    error_list[16] = "Index requires only 1 argument";
    error_list[17] = "Expected error pages argument on line";
    error_list[18] = "Expected cgi argument on line";
    error_list[19] = "Unexpected argument on line";
    error_list[20] = "Expected bodysize argument on line";
    error_list[21] = "Unexpected argument that cannot be converted to a number.";
    error_list[22] = "Expected file name on line";
    error_list[23] = "Unexpected argument on line";
    error_list[24] = "Unexpected allow method [GET / POST / HEAD / PUT / DELETE / TRACE / OPTIONS] on line";
    error_list[25] = "Expected methods argument on line";
    error_list[26] = "Expected  '}' on line";
    error_list[27] = "File is empty";
}

Error::~Error() { }

void Error::setAndPrint(const int &err_code, std::string const &line)
{
    size_t  flag = 0;
    this->err_code = err_code;
    
    for(std::map<int, std::string>::const_iterator it = error_list.begin(); it != error_list.end(); ++it)
    {
        if (it->first == this->err_code){
            if (line != "null" || !(line.empty()))
                cerr << BOLD_RED << "Error " << line << " " << it->first << ": " << it->second << RESET << endl;
            else
                cerr << BOLD_RED << "Error " << it->first << ": " << it->second << RESET << endl;
            flag++;
        }
    }
    if(flag == 0)
        cerr << BOLD_RED << "Unkown error code." << RESET << endl;
    exit(1);
}
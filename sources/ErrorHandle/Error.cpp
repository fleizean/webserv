#include "../../includes/Error.hpp"

Error::Error() : err_code(0) 
{
    initErrorValues();
}

Error::Error(const int& err_code) : err_code(err_code)
{
    initErrorValues();
}

Error::~Error() { }

void Error::initErrorValues()
{
    error_list[0] = "NULL.";
    error_list[1] = "Config file argument not found.";
    error_list[2] = "Could not open config file."; 
    error_list[3] = "Config file is empty.";
    error_list[4] = "Invalid file extension.";
    error_list[5] = "Invalid argument.";
    error_list[6] = "Please provide a config file.";
    error_list[7] = "Parenthesis is unbalanced.";
    error_list[8] = "Expected 'server {'.";
    error_list[9] = "Shouldn't be 'location'.";
    error_list[10] = "Location must be end with ';'.";
    error_list[11] = "Expected listen argument.";
    error_list[12] = "Expected server argument.";
    error_list[13] = "Expected root argument.";
    error_list[14] = "Root requires only 1 argument.";
    error_list[15] = "Expected index argument.";
    error_list[16] = "Index requires only 1 argument.";
    error_list[17] = "Expected error pages argument on line.";
    error_list[18] = "Expected cgi argument on line.";
    error_list[19] = "Unexpected argument on line.";
    error_list[20] = "Expected bodysize argument on line.";
    error_list[21] = "Unexpected argument that cannot be converted to a number.";
    error_list[22] = "Expected file name on line.";
    error_list[23] = "Expected argument on line.";
    error_list[24] = "Unexpected allow method [GET / POST / DELETE] on line.";
    error_list[25] = "Expected methods argument on line.";
    error_list[26] = "Expected  '}' on line.";
    error_list[27] = "File is empty.";
    error_list[28] = "Expected autoindex argument on line.";
    error_list[26] = "Expected  '{' on line.";
    error_list[29] = "Could not setup cluster.";
    error_list[30] = "Could not create server.";
    error_list[31] = "Could not bind port.";
    error_list[32] = "Could not listen.";
    error_list[33] = "Could not create socket.";
    error_list[34] = "There is no possible server.";
    error_list[35] = "Parse error from Request.";
    error_list[36] = "Socket setup failed.";
    error_list[37] = "Fcntl setup error.";
    error_list[38] = "setsockopt setup error.";
    error_list[39] = "bind function setup error.";
    error_list[40] = "listen socket error.";
    error_list[41] = "accept failed error.";
    error_list[42] = "read_connection failed error.";
    error_list[43] = "select function failed error.";
    error_list[44] = "HTTP protocol must be HTTP/1.1 in Http Request.";
    error_list[45] = "HTTP location must be in Http Request.";
    error_list[46] = "HTTP protocol must be in Http Request.";
    error_list[47] = "HTTP host must be in Http Request.";
    error_list[48] = "Port must be 1~65535 in Http Request.";
    error_list[49] = "HTTP accept language must be in Http Request.";
    error_list[50] = "Config file must have default location '/' path.";
    error_list[51] = "getcwd An error occurred while retrieving the current working directory.";
    error_list[52] = "Check validation error.";
    error_list[53] = "Wrong Data.";
    error_list[54] = "So Big Data.";
    error_list[55] = "Wrong ip address.";
    error_list[56] = "So big.";
    error_list[57] = "Read error, closing connection.";
}

void Error::setAndPrint(const int &err_code, std::string const &line)
{
    size_t  flag = 0;
    this->err_code = err_code;
    
    for (std::map<int, std::string>::const_iterator it = error_list.begin(); it != error_list.end(); ++it)
    {
        if (it->first == this->err_code){
            if (line != "null" || !(line.empty()))
                std::cout << BOLD_RED << "Error in [" << line << "] and value " << it->first << ": >" << it->second << "<" << RESET << endl;
            else
                std::cout << BOLD_RED << "Error " << it->first << ": " << it->second << RESET << endl;
            flag++;
        }
    }
    if (flag == 0)
        std::cout << BOLD_RED << "Unkown error code and message is " << line << " and code " << err_code << RESET << endl;
    exit(1);
}

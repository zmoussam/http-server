#include"Config.hpp"

Config::Config() {}
Config::Config(char *file)
{
    this->_file = file;
}
Config::~Config() {}
ServerConf::ServerConf() {
    this->_autoindex = false;
    this->_bodySize = 100000;
    this->_host = "127.0.0.1";
    this->_serverName = "";
    this->_listen = 8000;
    this->_root = "";
    this->_index = "";
    this->_uploadPath = "upload/";
    this->_errorPages = std::map<int , std::string>();
    this->_methods = std::vector<std::string>();
}
ServerConf::~ServerConf() {}
Location::Location() {
    this->_locationName = "";
    this->_returned = "";
    this->_compiler = "";
}
Location::~Location() {}

ServerConf::ServerConf(const ServerConf &copy)
{
    this->_autoindex = copy._autoindex;
    this->_bodySize = copy._bodySize;
    this->_host = copy._host;
    this->_serverName = copy._serverName;
    this->_listen = copy._listen;
    this->_root = copy._root;
    this->_index = copy._index;
    this->_errorPages = copy._errorPages;
    this->location = copy.location;
    this->_methods = copy._methods;
    this->_uploadPath = copy._uploadPath;
}

ServerConf &ServerConf::operator=(const ServerConf &copy)
{
    this->_autoindex = copy._autoindex;
    this->_bodySize = copy._bodySize;
    this->_host = copy._host;
    this->_serverName = copy._serverName;
    this->_listen = copy._listen;
    this->_root = copy._root;
    this->_index = copy._index;
    this->_errorPages = copy._errorPages;
    this->location = copy.location;
    this->_methods = copy._methods;
    this->_uploadPath = copy._uploadPath;
    return *this;
}

void Location::setReturned(std::string returned)
{
    this->_returned = returned;
}

void Location::setCompiler(std::string compiler)
{
    this->_compiler = compiler + " ";
}

std::string Location::getCompiler() const
{
    return this->_compiler;
}

std::string Location::getReturned() const
{
    return this->_returned;
}


void ServerConf::setMethods(std::vector<std::string> methods)
{
    this->_methods = methods;
}

std::vector<std::string> ServerConf::getMethods() const
{
    return this->_methods;
}
std::string Location::getLocationName() const
{
    return this->_locationName;
}

bool ServerConf::getAutoindex() const
{
    return this->_autoindex;
}

void ServerConf::setAutoindex(bool autoindex)
{
    this->_autoindex = autoindex;
}

void Location::setLocationName(std::string locationName)
{
    this->_locationName = locationName;
}

void ServerConf::setErrorPage(std::map<int , std::string>  errorPage) {
    this->_errorPages = errorPage;
}

std::map<int , std::string> ServerConf::getErrorPages() const {
    return this->_errorPages;
}

void ServerConf::setString(std::string type, std::string value)
{
    if (type == "server_name")
        this->_serverName = value;
    else if (type == "host")
        this->_host = value;
    else if (type == "root")
        this->_root = value;
    else if (type == "index")
        this->_index = value;
    else if (type == "upload_path")
        this->_uploadPath = value;
}

void ServerConf::setNum(std::string type, size_t value)
{
    if (type == "listen")
        this->_listen = value;
    else if (type == "client_body_size")
        this->_bodySize = value;
}

std::string ServerConf::getString(std::string type) const
{
    if (type == "server_name")
        return this->_serverName;
    else if (type == "host")
        return this->_host;
    else if (type == "root")
        return this->_root;
    else if (type == "index")
        return this->_index;
    else if (type == "upload_path")
    {
        if (this->_root[this->_root.length() - 1] != '/' && this->_uploadPath[0] != '/')
            return (this->_root + "/" + this->_uploadPath);
        else if (this->_root[this->_root.length() - 1] == '/' && this->_uploadPath[0] == '/')
            return (this->_root + this->_uploadPath.substr(1));
        else  
            return (this->_root + this->_uploadPath);
    }
    return "";
}

size_t ServerConf::getNum(std::string type) const
{
    if (type == "listen")
        return this->_listen;
    else if (type == "client_body_size")
        return this->_bodySize;
    return 0;
}

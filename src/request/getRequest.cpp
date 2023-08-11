#include "Request.hpp"


std::string Request::getFullRequest() const
{
    return this->_buffer;
}

size_t Request::getRequestLength() const
{
    return this->bufferLength;
}

std::string Request::getBody() const
{
    return this->_body;
}

std::string Request::getHTTPVersion() const
{
    return this->_httpVersion;
}

std::string Request::getPath() const
{
    return this->_path;
}

std::string Request::getMethod() const
{
    return this->_method;
}

std::string Request::getQueries() const
{
    return this->_queries;
}

bool Request::KeepAlive() const
{
    return this->_keepAlive;
}

std::map<std::string, std::string> Request::getHeaders() const
{
    return this->_headers;
}

std::map<std::string, std::string> Request::getCookies() const
{
    return this->_cookies;
}
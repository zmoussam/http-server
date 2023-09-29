#include "Request.hpp"

Request::Request(const Request & other) {
    *this = other;
}
Request &Request::operator=(const Request & other) {
    if (this != &other) {
        this->_REQ.write(other._REQ.str().c_str(), other._REQ.str().size());
        this->_request = other._request;
        this->_requestLength = other._requestLength;
        this->_httpVersion = other._httpVersion;
        this->_body = other._body;
        this->_URI = other._URI;
        this->_method = other._method;
        this->_queries = other._queries;
        this->_boundary = other._boundary;
        this->_boundaryBody = other._boundaryBody;
        this->_headers = other._headers;
        this->_cookies = other._cookies;
        this->_config = other._config;
        this->_servers = other._servers;
        this->_bodySize = other._bodySize;
        this->_error = other._error;
        this->_keepAlive = other._keepAlive;
        this->_isHeadersRead = other._isHeadersRead;
        this->_clientSocket = other._clientSocket;
        this->_isBodyRead = other._isBodyRead;
        this->_checkBoundary = other._checkBoundary;
    }
    return *this;
}

std::string Request::getFullRequest() const
{
    return this->_request;
}

size_t Request::getRequestLength() const
{
    return this->_requestLength;
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
    return this->_URI;
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


bool Request::isHeadersRead() const {
	return _isHeadersRead;
}

bool Request::isBodyRead() const {
	return _isBodyRead;
}

size_t Request::getBodyLength(std::string Content_length)
{
    std::string bodylength = "";
    int i = 0;
    while (Content_length[i] != '\r' && std::isdigit(Content_length[i]))
    {
        bodylength += Content_length[i];
        i++;
    }
    std::istringstream converter(bodylength);
    long long bodyLength;
    if (converter >> bodyLength)
        return bodyLength;
    else {
        _error = 400;
        return 0;
    };
}

int hexStringToInt(const std::string hexString) {
    std::istringstream converter(hexString);
    int intValue;
    converter >> std::hex >> intValue;
    return intValue;
}

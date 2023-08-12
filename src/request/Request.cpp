/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:46:08 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/12 19:35:10 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <sys/socket.h>
#include <sstream>

Request::Request(int clientSocket) 	: 
	_buffer(""),
    bufferLength(0),
    _httpVersion(""),
	_body(""),
	_URI(""),
    _method(""),
	_queries(),
	_headers(),
	_cookies(),
	_keepAlive(1)
{
    readRequest(clientSocket);
    bufferLength = _buffer.size();
    
}

void Request::readRequest(int client_socket)
{
    char buffer[11];
    int rd = 1;
    while (rd != 0)
    {
        rd = recv(client_socket, buffer, 10, 0);
        if (rd < 0)
        {
            std::cout << "Error : recieve request failed \n" << strerror(errno) << std::endl;
            return;
        }
        buffer[rd] = '\0';
        _buffer += std::string(buffer);
    }
    // std::cout << _buffer << std::endl;
}

void Request::parsseRequest()
{
    size_t nextPos = 0;
    parsseMethod(nextPos);
    parssePath_Queries(nextPos);
    parsseHTTPversion(nextPos);
    parsseHeaders(nextPos);
    parsseBody(nextPos);
    parsseCookies();
}

void Request::parsseMethod(size_t &methodPos)
{
    _method = "";
    for (; methodPos < bufferLength && _buffer[methodPos] != ' '; methodPos++)
        _method += _buffer[methodPos];
}

void Request::parssePath_Queries(size_t &URI_Pos)
{
    std::string URI = "";
    size_t queryPos;
    for (; URI_Pos < bufferLength && _buffer[URI_Pos] == ' '; URI_Pos++);
    for (; URI_Pos < bufferLength && _buffer[URI_Pos] != ' '; URI_Pos++)
        URI += _buffer[URI_Pos];
    queryPos = URI.find('?');
    if (queryPos != std::string::npos)
    {
        _URI = URI.substr(0, queryPos);
        _queries = URI.substr(queryPos + 1);
    }
    else {
        _URI = URI;
    }
}

void Request::parsseHTTPversion(size_t &_httpversion_Pos)
{
    _httpVersion = "";
    for (; _httpversion_Pos < bufferLength && _buffer[_httpversion_Pos] == ' '; _httpversion_Pos++);
    for (; _httpversion_Pos < bufferLength && _buffer[_httpversion_Pos] != ' ' \
       && _buffer[_httpversion_Pos] != '\r'; _httpversion_Pos++)
        _httpVersion += _buffer[_httpversion_Pos];
}

void Request::parsseHeaders(size_t &_hpos)
{
    std::string _key;
    size_t _headerkeyPos;
    size_t _headerValuePos;
    _keepAlive = false;
    size_t bodypos = _buffer.find("\r\n\r\n", _hpos);
    if (bodypos == std::string::npos)
        return;
    while (_hpos < bodypos)
    {
        _headerkeyPos = _buffer.find(':', _hpos);
        if (_headerkeyPos == std::string::npos)
            break;
        _key = _buffer.substr(_hpos + 2, _headerkeyPos - _hpos - 2);
        _headerValuePos = _buffer.find("\r\n", _headerkeyPos);
        _headers[_key] = _buffer.substr(_headerkeyPos + 2, _headerValuePos - _headerkeyPos - 2);
        if (_key == "Connection" && _headers[_key] == "keep-alive")
            _keepAlive = true;
        _hpos = _headerValuePos;
        std::cout << '$' << _key << "$ : " << '$' << this->_headers[_key]  << "$" << std::endl;
    }
}

void Request::parsseCookies()
{
    std::string _key;
    std::string _value;
    std::string Cookies;
    if (_headers.find("Cookie") != _headers.end() || _headers.find("Cookies") != _headers.end()) {
        if (_headers.find("Cookie") != _headers.end()) { Cookies = _headers["Cookie"]; }
        else { Cookies = _headers["Cookies"]; }
        size_t CookiesLength = Cookies.size();
        for (size_t i = 0; i < CookiesLength; i++) {
            _key = "" , _value = "";
            for (; i < CookiesLength && Cookies[i] != '='; i++)
                if (Cookies[i] != ' ')
                    _key +=  Cookies[i];
            i++;
            for (; i < CookiesLength && Cookies[i] != ';'; i++)
                if (Cookies[i] != ' ')
                    _value += Cookies[i];
            _cookies[_key] = _value;
        }
        _headers.erase("Cookie");
        _headers.erase("Cookies");
    }
}

void Request::parsseBody(size_t &_bodyPos)
{
    _body = "";
    if (_bodyPos == std::string::npos)
        return;
    if (_bodyPos + 4 < bufferLength - 1)
        _body = _buffer.substr(_bodyPos  + 4);
    std::cout << _body.size() << "  \n" <<  _body << std::endl;
}

Request::~Request()
{
    _headers.clear();
    _queries.clear();
    _cookies.clear();
}
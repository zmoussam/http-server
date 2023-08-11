/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:46:08 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/11 22:17:19 by zmoussam         ###   ########.fr       */
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
	_path(""),
    _method(""),
	_queries(),
	_headers(),
	_cookies(),
	_keepAlive(1)
{
    int r_bytes;
    char tmpBuffer[BUFFER_SIZE + 1];
    this->_clientSocket = clientSocket;
    r_bytes = recv(clientSocket, tmpBuffer, BUFFER_SIZE, 0);
    if (r_bytes < 0)
        std::cout << "Error : received data failed" << std::endl;
    else
        tmpBuffer[r_bytes + 1] = '\0';
    _buffer = std::string(tmpBuffer);
    // std::cout << _buffer << std::endl;
    bufferLength = _buffer.size();
    
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

void Request::parssePath_Queries(size_t &URL_Pos)
{
    std::string URL = "";
    size_t queryPos;
    for (; URL_Pos < bufferLength && _buffer[URL_Pos] == ' '; URL_Pos++);
    for (; URL_Pos < bufferLength && _buffer[URL_Pos] != ' '; URL_Pos++)
        URL += _buffer[URL_Pos];
    queryPos = URL.find('?');
    if (queryPos != std::string::npos)
    {
        _path = URL.substr(0, queryPos);
        _queries = URL.substr(queryPos + 1);
    }
    else {
        _path = URL;
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
    size_t _blankLine; 
    _keepAlive = false;
    while (_hpos < bufferLength)
    {
        _blankLine = 0;
        while(_buffer[_hpos] && (_buffer[_hpos] == '\r' || _buffer[_hpos] == '\n'))
        {
            if (++_blankLine == 4)
                return;
            _hpos++;
        }
        _headerkeyPos = _buffer.find(':', _hpos);
        if (_headerkeyPos == std::string::npos)
            break;
        _key = _buffer.substr(_hpos, _headerkeyPos - _hpos);
        _headerValuePos = _buffer.find("\r\n", _headerkeyPos);
        _headers[_key] = _buffer.substr(_headerkeyPos + 2, _headerValuePos - _headerkeyPos - 2);
        if (_key == "Connection" && _headers[_key] == "keep-alive")
            _keepAlive = true;
        _hpos = _headerValuePos;
        // std::cout << '$' << _key << "$ : " << '$' << this->_headers[_key]  << "$" << std::endl;
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
    if (_bodyPos < bufferLength - 1)
        _body = _buffer.substr(_bodyPos  + 1, bufferLength - _bodyPos - 2);
    // std::cout << _body.size() << "  " <<  _body << std::endl;
}

Request::~Request()
{
    _headers.clear();
    _queries.clear();
    _cookies.clear();
}
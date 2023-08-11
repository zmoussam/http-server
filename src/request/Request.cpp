/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:46:08 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/11 19:49:54 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Request.hpp"
#include <sys/socket.h>
#include <sstream>

Request::Request(int clientSocket)
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
    getMethod(nextPos);
    getPath_Querys(nextPos);
    getHTTPversion(nextPos);
    getHeader_body(nextPos);
    // getBody(nextPos);
    getCookies();
}

void Request::getMethod(size_t &methodPos)
{
    _method = "";
    for (; methodPos < bufferLength && _buffer[methodPos] != ' '; methodPos++)
        _method += _buffer[methodPos];
}

void Request::getPath_Querys(size_t &URL_Pos)
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
        _querys = URL.substr(queryPos + 1);
    }
    else {
        _path = URL;
    }
}

void Request::getHTTPversion(size_t &_httpversion_Pos)
{
    _httpVersion = "";
    for (; _httpversion_Pos < bufferLength && _buffer[_httpversion_Pos] == ' '; _httpversion_Pos++);
    for (; _httpversion_Pos < bufferLength && _buffer[_httpversion_Pos] != ' ' \
       && _buffer[_httpversion_Pos] != '\r'; _httpversion_Pos++)
        _httpVersion += _buffer[_httpversion_Pos];
}

void Request::getHeader_body(size_t &_hpos)
{
    std::string _key;
    size_t _headerkeyPos;
    size_t _headerValuePos;
    size_t _blankLine; 
    _keepAlive = 0;
    while (_hpos < bufferLength)
    {
        _blankLine = 0;
        while(_buffer[_hpos] && (_buffer[_hpos] == '\r' || _buffer[_hpos] == '\n'))
        {
            _blankLine++;
            if (_blankLine == 4)
                break;
            _hpos++;
        }
        if (_blankLine == 4)
        {
            std::cout << "okey  !" << std::endl;
            break;
        }
        _headerkeyPos = _buffer.find(':', _hpos);
        if (_headerkeyPos == std::string::npos)
            break;
        _key = _buffer.substr(_hpos, _headerkeyPos - _hpos);
        _headerValuePos = _buffer.find("\r", _headerkeyPos);
        _header[_key] = _buffer.substr(_headerkeyPos + 2, _headerValuePos - _headerkeyPos - 2);
        if (_key == "Connection" && _header[_key] == "keep-alive")
            _keepAlive = 1;
        _hpos = _headerValuePos;
        std::cout << '$' << _key << "$ : " << '$' << this->_header[_key]  << "$" << std::endl;
    }
    if (_hpos == bufferLength + 1)
        _body = "";
    else 
        _body = _buffer.substr(_hpos + 1, bufferLength - _hpos - 2);
    std::cout << _body << std::endl;
}

void Request::getCookies()
{
    std::string _key;
    std::string _value;
    std::string Cookies;
    if (_header.find("Cookie") != _header.end() || _header.find("Cookies") != _header.end()) {
        if (_header.find("Cookie") != _header.end()) { Cookies = _header["Cookie"]; }
        else { Cookies = _header["Cookies"]; }
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
        _header.erase("Cookie");
        _header.erase("Cookies");
    }
}

Request::~Request()
{
    _header.clear();
    _querys.clear();
    _cookies.clear();
}
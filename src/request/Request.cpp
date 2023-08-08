/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:46:08 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/08 22:34:32 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Request.hpp"
#include <sys/socket.h>

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
    // std::cout << "next : " << nextPos << std::endl;
    getURL(nextPos);
    getHTTPversion(nextPos);
    getHeader(nextPos);
    // getBody();
}

void Request::getMethod(size_t &methodPos)
{
    _method = "";
    for (; methodPos < bufferLength && _buffer[methodPos] != ' '; methodPos++)
        _method += _buffer[methodPos];
    // std::cout << "method : " << _method << std::endl;
}

void Request::getURL(size_t &URL_Pos)
{
    _URL = "";
    for (; URL_Pos < bufferLength && _buffer[URL_Pos] == ' '; URL_Pos++);
    for (; URL_Pos < bufferLength && _buffer[URL_Pos] != ' '; URL_Pos++)
        _URL += _buffer[URL_Pos];
    // std::cout << "Path : " << _URL << std::endl;
}

void Request::getHTTPversion(size_t &_httpversion_Pos)
{
    _httpVersion = "";
    for (; _httpversion_Pos < bufferLength && _buffer[_httpversion_Pos] == ' '; _httpversion_Pos++);
    for (; _httpversion_Pos < bufferLength && _buffer[_httpversion_Pos] != ' ' \
       && _buffer[_httpversion_Pos] != '\r'; _httpversion_Pos++)
        _httpVersion += _buffer[_httpversion_Pos];
    // std::cout << "http : " << _httpVersion << std::endl;
}

void Request::getHeader(size_t &header_pos)
{
    std::string _key;
    size_t _headerkeyPos;
    size_t _headerValuePos;
    size_t i = header_pos;
    _keepAlive = 0;
    while (i < this->bufferLength)
    {
        if (_buffer[i] && (_buffer[i] == ' ' || _buffer[i] == '\r' || _buffer[i] == '\n'))
            i++;
        else {
            _headerkeyPos = _buffer.find(':', i);
            if (_headerkeyPos == std::string::npos)
                return;
            _key = _buffer.substr(i, _headerkeyPos - i);
            _headerValuePos = _buffer.find("\r", _headerkeyPos);
            this->_header[_key] = _buffer.substr(_headerkeyPos + 2, _headerValuePos - _headerkeyPos - 2);
            if (_key == "Connection" && _header[_key] == "keep-alive")
                _keepAlive = 1;
            // std::cout << '$' << _key << "$ : " << '$' << this->_header[_key]  << "$" << std::endl;
            i = _headerValuePos;
        }
    }
}

Request::~Request()
{
    
}
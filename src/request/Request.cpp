/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:46:08 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/07 21:17:13 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Request.hpp"
#include <sys/socket.h>

Request::Request(int clientSocket)
{
    int r_bytes;
    this->_clientSocket = clientSocket;
    r_bytes = recv(clientSocket, this->_buffer, BUFFER_SIZE, 0);
    if (r_bytes < 0)
        std::cout << "Error : received data failed" << std::endl;
    else
        _buffer[r_bytes + 1] = '\0';
    this->bufferLength = std::string(this->_buffer).size();
    
}
void Request::parsseRequest()
{
    size_t nextPos = 0;
    getMethod(nextPos);
    std::cout << "next : " << nextPos << std::endl;
    getURL(nextPos);
    getHTTPversion(nextPos);
    getHeader();
    // getBody();
}

void Request::getMethod(size_t &methodPos)
{
    this->_method = "";
    for (; methodPos < this->bufferLength && _buffer[methodPos] != ' '; methodPos++)
        this->_method += _buffer[methodPos];
    // std::cout << "method : " << this->_method << std::endl;
}

void Request::getURL(size_t &URL_Pos)
{
    this->_URL = "";
    for (; URL_Pos < this->bufferLength && this->_buffer[URL_Pos] == ' '; URL_Pos++);
    for (; URL_Pos < this->bufferLength && this->_buffer[URL_Pos] != ' '; URL_Pos++)
        this->_URL += this->_buffer[URL_Pos];
    // std::cout << "Path : " << this->_URL << std::endl;
}

void Request::getHTTPversion(size_t &_httpversion_Pos)
{
    this->_httpVersion = "";
    for (; _httpversion_Pos < this->bufferLength && this->_buffer[_httpversion_Pos] == ' '; _httpversion_Pos++);
    for (; _httpversion_Pos < this->bufferLength && this->_buffer[_httpversion_Pos] != ' ' \
       && this->_buffer[_httpversion_Pos] != '\r'; _httpversion_Pos++)
        this->_httpVersion += this->_buffer[_httpversion_Pos];
    // std::cout << "http : " << this->_httpVersion << std::endl;
}

Request::~Request()
{
    
}
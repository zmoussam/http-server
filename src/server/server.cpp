/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 17:38:52 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/12 18:15:58 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string>

#define LOCAL_IP 2130706433

server::server(const short _Port)
{
    int reuse = 1;
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->_serverSocket < 0)
    {
        std::cout << "Error : can't creat socket \n- " << strerror(errno) << std::endl;
        std::exit(1);
    }
    
    memset((char *)&this->address, 0 , sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = htonl(INADDR_ANY);
    this->address.sin_port = htons(_Port);

    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) < 0) {
        std::cerr << "Error: setsockopt failed \n" << strerror(errno) << std::endl;
        std::exit(1);
    }
    
    if (bind(_serverSocket, (struct sockaddr *)&this->address, (socklen_t)sizeof(this->address)) < 0)
    {
        std::cout << "Error : can't bind address \n- " << strerror(errno) << std::endl;
        std::exit(1);
    }
}


void server::start() const
{
    int client_socket = -1;
    socklen_t socklen = (socklen_t)sizeof(this->address);
    std::string line;
    if (listen(this->_serverSocket, 1000) < 0)
    {
        std::cout << "Error : server can't listen to incoming connection \n- " << strerror(errno) << std::endl;
        std::exit(1);
    }
    while(1)
    {
        client_socket = accept(this->_serverSocket, (struct sockaddr *)&this->address, &socklen);
        if (client_socket < 0)
        {
            std::cout << "Error : can't accept connection \n- " << strerror(errno) << std::endl;
            continue;
        }
        // readRequest(client_socket);
        Request _request(client_socket);
        _request.parsseRequest();
        // Response res(client_socket);
        close(client_socket);
    }
}

server::~server()
{

}
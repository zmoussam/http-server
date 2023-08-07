/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 18:26:01 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/05 19:27:06 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// // #include "../../inc/server.hpp"
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// #include <errno.h>
// #include <iostream>
// int main()
// {
//     struct  sockaddr_in address;
    
//     int  _serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if ( _serverSocket == -1)
//     {
//         std::cout << "Error : can't creat socket \n- " << strerror(errno) << std::endl;
//     }
//     memset((char *)&address, 0 , sizeof(address));
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = htonl(INADDR_ANY);
//     address.sin_port = htons(8080);
//     if (bind(_serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
//     {
//         std::cout << "Error : can't bind address \n- " << strerror(errno) << std::endl;
//         std::exit(1);
//     }
//     int  client_socket = accept(_serverSocket, (struct sockaddr *)&address, (socklen_t*)sizeof(address));
//     if (client_socket < -1)
//     {
//         std::cout << "Error : can't accept connection \n- " << strerror(errno) << std::endl;
//         std::exit(1);
//     }
// }
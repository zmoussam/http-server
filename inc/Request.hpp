/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:29:36 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/08 00:58:35 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  REQUEST__
#define REQUEST__
#include <iostream>
#include <string>
#define BUFFER_SIZE 3000
    class Request
    {
        private:
            int _clientSocket;
            char _buffer[BUFFER_SIZE + 1];
            size_t bufferLength;
            std::string _httpVersion;
            std::string _body;
            std::string _header;
            std::string _URL;
            std::string _method;
        public:
            Request(int client_socket);
            ~Request();
            void parsseRequest();
            void getMethod(size_t &methodPos);
            void getURL(size_t &URL_Pos);
            void getHTTPversion(size_t &httpVersion_pos);
            void getHeader(size_t &header_pos);
            void getBody();
    };
    
#endif
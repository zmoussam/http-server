/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:29:36 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/08 17:20:31 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  REQUEST__
#define REQUEST__
#include <iostream>
#include <string>
#include <map>
#define BUFFER_SIZE 3000
    class Request
    {
        private:
            int _clientSocket;
            std::string _buffer;
            size_t bufferLength;
            std::string _httpVersion;
            std::string _body;
            std::map<std::string, std::string> _header;
            std::string _URL;
            std::string _method;
            int _keepAlive;
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
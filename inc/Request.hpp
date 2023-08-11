/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 21:29:36 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/11 22:25:36 by zmoussam         ###   ########.fr       */
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
            std::string _path;
            std::string _method;
            std::string _queries;
            std::map<std::string, std::string> _headers;
            std::map<std::string , std::string> _cookies;
            bool _keepAlive;
        public:
            std::string getFullRequest() const;
            size_t getRequestLength() const;
            std::string getBody() const;
            std::string getHTTPVersion() const;
            std::string getPath() const;
            std::string getMethod() const;
            std::string getQueries() const;
            bool KeepAlive() const;
            std::map<std::string, std::string> getHeaders() const;
            std::map<std::string, std::string> getCookies() const;
            void parsseRequest();
            void parsseMethod(size_t &methodPos);
            void parssePath_Queries(size_t &URL_Pos);
            void parsseHTTPversion(size_t &httpVersion_pos);
            void parsseHeaders(size_t &header_pos);
            void parsseBody(size_t &bodyPos);
            void parsseCookies();
            Request(int client_socket);
            ~Request();
    };
    
#endif
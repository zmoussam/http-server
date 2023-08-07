/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 17:39:27 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/05 19:29:24 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER__
#define SERVER__
#include <iostream>
#include <string>
#include <netinet/in.h>
    class server
    {
        private:
            int _serverSocket;
            struct  sockaddr_in address;

        public:
            server(const short _Port);
            ~server();
            void start() const;
            
    };

#endif     
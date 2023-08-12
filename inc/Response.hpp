/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmoussam <zmoussam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 17:24:40 by zmoussam          #+#    #+#             */
/*   Updated: 2023/08/12 17:30:37 by zmoussam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE__
#define RESPONSE__

    class Response
    {
        private:
            /* data */
        public:
            Response(int client_socket);
            ~Response();
    };
    
#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GET_request.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toni <toni@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 06:08:52 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 16:12:15 by toni             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_REQUEST_HPP
#define GET_REQUEST_HPP

#include "HTTPRequest.hpp"

/**
 * @brief GET TCP - Allows server to respond to incoming GET requests
 */
class GET_request : public HTTPRequest
{
    public:
        GET_request(std::istringstream &request_data, servers_map &s_map, int socket_fd) : HTTPRequest(request_data, s_map, socket_fd) {}
        ~GET_request() {}
        virtual void action();

    private:
        void list_directory_page(locations::iterator location);
        
        void handle_cgi_request(CGI_map::iterator cgi_location, locations::iterator location);
        
};

#endif  // GET_REQUEST_HPP
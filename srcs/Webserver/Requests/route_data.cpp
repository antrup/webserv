/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   route_data.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 21:47:37 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/14 23:23:32y sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Route incomming data to the relevant function
 */

void Webserver::route_incoming_data(int socket_id)
{  
    std::cout << YELLOW "Routing Data ..." RESET << std::endl;
    // Check existing requests
    incoming_map::iterator incoming_it = _data_incoming_list.find(_sockets[socket_id].fd);

    // if NO existing request
    if (!incoming_it->second)
        initial_peek(socket_id);
    else
    {
        HTTPRequest *request  = incoming_it->second;
        additionnal_peek(socket_id, static_cast<POST_request*>(request));
        handle_request(request, socket_id);   
    }
}

/**
 * @brief Handle bad request : prepare closure of incomming socket + generation of error message
 */

void    Webserver::handle_bad_request(int socket_id, int return_code)
{
    int ret = 1;
    while (ret)
    {
        char garbage[BUFFER_SIZE];
        ret = recv(_sockets[socket_id].fd, garbage, BUFFER_SIZE, 0);
        if (ret < BUFFER_SIZE)
            ret = 0;
    }
    incoming_map::iterator to_close = _data_incoming_list.find(_sockets[socket_id].fd);
    _data_incoming_list.erase(to_close);
    create_error_response(return_code, socket_id);
}


/**
 * @brief get and transfer incoming data to existing HTTP request
 * 
 * @param socket_id socket we are using
 */

void Webserver::get_additionnal_data(int socket_id, size_t data_size, POST_request *request)
{
    char buffer[data_size];
    memset(&buffer, 0, data_size);
    recv(_sockets[socket_id].fd, buffer, data_size, 0);

    std::istringstream str_buffer(buffer);
    request->get_body(str_buffer);
    request->push_data_to_fd();
}
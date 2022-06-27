/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_request.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:42:31 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 21:11:29 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief receive request for parsing
 * 
 * @param socket_id socket we are using
 */

void    Webserver::create_request(int socket_id, size_t request_size, int type)
{
    char buffer[request_size + 1];
    memset(&buffer, 0, request_size + 1);
    int n;
    n = recv(_sockets[socket_id].fd, buffer, request_size, 0);
    if (n == -1)
    {
        data_socket_error(socket_id);
        return;
    }
    buffer[n] = '\0';

    // LOG and DEBUG purposes
    std::cout << YELLOW << "Incoming request" << RESET << std::endl;
    std::cout << buffer << std::endl;
    std::cout << YELLOW << "End incoming" << RESET << std::endl;
    
    std::istringstream request(buffer);
    
    // Create HTTPrequest
    HTTPRequest *http_request = NULL;
    // GET
    if (type == M_GET)
        http_request = new GET_request(request, _servers, _sockets[socket_id].fd);
    // POST
    if (type == M_POST)
        http_request = new POST_request(request, _servers, _sockets[socket_id].fd);
    // DELETE
    if (type == M_DELETE)
        http_request = new DELETE_request(request, _servers, _sockets[socket_id].fd);

    handle_request(http_request, socket_id);
}
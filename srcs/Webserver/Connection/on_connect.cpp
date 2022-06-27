/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_connect.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:42:31 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/14 18:10:29 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief accept incoming connection and create pollfd
 * 
 * @param socket_id socket that the connection is received on
 */

void Webserver::on_client_connect(int socket_id)
{
    int new_connection;
    new_connection = accept(_sockets[socket_id].fd, NULL, NULL);
    if (new_connection < 0)
        perror("New connection failed");
    struct pollfd new_pollfd;
    memset(&new_pollfd, 0, sizeof(new_pollfd));
    new_pollfd.fd = new_connection;
    new_pollfd.events = POLLIN | POLLOUT | POLLHUP | POLLERR;
    _data_incoming_list.insert(std::pair<const int, HTTPRequest*>(new_connection, NULL));
    _sockets.push_back(new_pollfd);
}
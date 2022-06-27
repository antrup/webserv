/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:16:21 by toni              #+#    #+#             */
/*   Updated: 2022/05/21 17:25:54 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief close and delete disconnected data sockets
 * 
 * @param socket_id socket that the connection is received on
 */

void Webserver::close_data_socket(int socket_id)
{
    incoming_map::iterator to_close = _data_incoming_list.find(_sockets[socket_id].fd);
    if (to_close != _data_incoming_list.end())
        _data_incoming_list.erase(to_close);
    close(_sockets[socket_id].fd);
    _sockets.erase(_sockets.begin() + socket_id);
}
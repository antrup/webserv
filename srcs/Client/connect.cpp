/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 18:08:13 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/16 13:52:20 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/**
 * @brief Connect Method - connects Client to Server
 * 
 */

void HTTPClient::connect()
{
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        // Create socket
        connections[i].socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (connections[i].socket_fd < 0)
        {
            // FAILURE
            std::cout << RED << "Failed to create socket" << std::endl;
            std::cout << "EXIT" << RESET << std::endl;
            exit(1);
        }
        else
            // SUCCESS
            std::cout << GREEN "Socket created" RESET << std::endl;
        
        // assign IP and PORT
        connections[i].server_address.sin_family = AF_INET;
        connections[i].server_address.sin_addr.s_addr = inet_addr(connections[i].IP_address.c_str());
        connections[i].server_address.sin_port = htons(connections[i].port);
        
        // Connect client socket to server socket
        if (::connect(connections[i].socket_fd, (SA*)&connections[i].server_address, sizeof(connections[i].server_address)) != 0)
        {
            // FAILURE
            std::cout << RED << "Failed to connect to server socket";
            std::cout << "EXIT" << RESET << std::endl;
            exit(1);
        }
        else
        {
            // SUCCESS
            std::cout << GREEN << "CONNECTED " << RESET;
            std::cout << "via port ";
            std::cout << YELLOW << connections[i].port;
            std::cout << BLUE << " [ FD " << connections[i].socket_fd << "]" << RESET << std::endl;
            // ADD and setup pollfd
            struct pollfd tmp;
            memset(&tmp, 0, sizeof(tmp));
            tmp.fd = connections[i].socket_fd;
            tmp.events = POLLIN | POLLOUT | POLLHUP;
            poll_sockets.push_back(tmp);
        }
    }
}
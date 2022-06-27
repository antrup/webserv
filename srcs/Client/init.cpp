/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_client.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 14:35:59 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/12 17:01:31 sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/**
 * @brief Helper function to ietarte initialisation
 * 
 * @param client    structure we want to initialize
 * @param IP        IP we want to connet with
 * @param host_name unique hostname
 * @param port      port to connect to
 */

void init_client_structures(client_id *client, std::string IP, std::string host_name, unsigned int port)
{
    client->IP_address = IP;
    client->host_name = host_name;
    client->port = port;
}

/**
 * @brief Initialize the connection
 * 
 */
void HTTPClient::init()
{
    // INIT STRUCTURES
    client_id tmp;
    memset(&tmp, 0, sizeof(tmp));
    
    // PORT 8887 till MAX_CONNECTIONS
    unsigned int    port = 8887;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        init_client_structures(&tmp, "127.0.0.1", "localhost", port++);
        connections.push_back(tmp);
    }

}

/**
 * @brief disconnect and close socket
 * 
 */
void HTTPClient::disconnect()
{
    std::cout << "Disconnecting from server" << std::endl;
    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        std::cout << "Disconnecting port : " << connections[i].port << std::endl;
        close(connections[i].socket_fd);
    }
    std::cout << "Disconnected from server" << std::endl;
}

/**
 * @brief Construct a new HTTPClient::HTTPClient object
 * 
 */

HTTPClient::HTTPClient()
{
    std::cout << GREEN "HTTPClient -- STARTING" RESET << std::endl;
}

/**
 * @brief Destroy the HTTPClient::HTTPClient object
 * 
 */

HTTPClient::~HTTPClient()
{
    std::cout << GREEN "HTTPClient -- ENDING" RESET << std::endl;
}
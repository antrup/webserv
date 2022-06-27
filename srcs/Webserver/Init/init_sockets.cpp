/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sockets.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 18:03:38 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/21 14:57:18 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Initialize the sockets and necessary data structures
 */

void Webserver::init()
{
    servers_map::iterator server_id = _servers.begin();
    while (server_id != _servers.end())
    {
        hosts_map::iterator host_id;
        
        if ((host_id = server_id->second.find("0.0.0.0")) != server_id->second.end())
            init_socket(server_id->first, "0.0.0.0");
        else
        {
            host_id = server_id->second.begin();
            while (host_id != server_id->second.end())
            {
                init_socket(server_id->first, host_id->first);
                host_id++;
            }
        }
        server_id++;
    }
}        

/**
 * @brief Retry creation of listen sockets
 */

void Webserver::init_retry()
{
    to_init_sockets::iterator it;
    to_init_sockets list(_init_sockets);
    
    _init_sockets.clear();

    it = list.begin();
    while (it != list.end())
    {
        init_socket(it->first, it->second);
        it++;
    }
}

/**
 * @brief Creation of listen sockets
 */

void Webserver::init_socket(std::string port, std::string host)
{
    // Open a new socket to listen on
    int new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (new_socket == 0)
    {
        std::cout << "Socket creation failed. Retry in 10s" << std::endl;
        sleep(10);
        return (init_socket(port, host));
    }
    // Create a new pollfd structure
    struct pollfd new_pollfd;
    memset(&new_pollfd, 0, sizeof(new_pollfd));
    new_pollfd.fd = new_socket;
    new_pollfd.events = POLLIN;
    _sockets.push_back(new_pollfd); // Add the structure to sockets
    // Setup hints for socket address bind
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    // Setup socket address information
    struct addrinfo *res;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
    {    
        std::cout << "Socket creation failed for " << host << ":" << port << " invalid port or host name" << std::endl;
        _sockets.pop_back();
        close(new_socket);
        return;
    } 
    socket_addr_info.push_back(res);
    int optval = 1;
    if (setsockopt(new_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval)) == -1)
    {
        std::cout << " c la merde " << std::endl;
    }
    // Bind sockets
    if (bind(_sockets.back().fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        std::cout << "Socket bind failed for " << host << ":" << port << ". Retry in 30s " << std::endl;
        _sockets.pop_back();
        close(new_socket);
        _init_sockets.push_back(std::make_pair(port, host));
        return;
    }
    // Start listening on socket
    if (listen(_sockets.back().fd, MAX_BACKLOG) < 0)
    {
        std::cout << "Socket listening failed for " << host << ":" << port << ". Retry in 30s " << std::endl;
        _sockets.pop_back();
        close(new_socket);
        _init_sockets.push_back(std::pair<std::string, std::string>(port, host));
        return;
    }
    
    _listen_socket.insert(std::make_pair(new_socket, std::make_pair(port, host)));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_block_matching.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 05:16:50 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/15 23:04:23 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

locations::iterator HTTPRequest::match_location()
{
    size_t pos;
    std::string match;
    locations::iterator location_id;
    std::cout << YELLOW <<  "match location -- start" << RESET << std::endl;
    for (locations::iterator it = _server->locations.begin(); it != _server->locations.end(); it++)
    {
        pos = _headers["URL"].find(it->location);
        if (pos != std::string::npos && it->location.size() > match.size())
        {
            match = _headers["URL"].substr(pos, it->location.size());
            std::cout << BLUE << "matching locations -> " << RESET;
            std::cout << CYAN << match << RESET << std::endl;
            location_id = it;
        }
    }
    std::cout << GREEN << "best match -> " << RESET;
    std::cout << CYAN << match << std::endl;
    std::cout << YELLOW <<  "match location -- end" << RESET << std::endl;
    return location_id;
}

t_server_block *HTTPRequest::get_server_block(std::string name, int fd, servers_map &servers_map)
{
    struct sockaddr_in addr_v4 = http_tools::get_ipv4_addr(fd);
    std::string host = http_tools::get_host(&addr_v4);
    std::string port = http_tools::get_port(&addr_v4);
    
    std::cout << BLUE << "Host IP : " << RESET;
    std::cout << CYAN  << host;
    std::cout << RESET << ":" << PURPLE << port;
    std::cout << RESET << std::endl;
    
    servers_map::iterator server_id;
    hosts_map::iterator host_id;
    server_names_map::iterator block_id;
    
    server_id = servers_map.find(port);
    host_id = server_id->second.find(host);
    if ((block_id = host_id->second.find(name)) != host_id->second.end())
        return &block_id->second;
    else
    {
        block_id = host_id->second.begin();
        while (block_id != host_id->second.end())
        {
            if (block_id->second.is_default)
                return &block_id->second;
            block_id++;
        }
    }
    perror(" block getter tout casse");
    return NULL;
}
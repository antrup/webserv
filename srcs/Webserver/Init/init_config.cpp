/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 17:42:31 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/21 14:24:55 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Construct a new Webserver object
 *
 * @param config_file config for server
 */

Webserver::Webserver(std::string config) : _parsing(config.c_str())
{
    servers_map::iterator it;
    hosts_map::iterator itt;
    server_names_map::iterator ittt;
    std::vector<t_server_block>::iterator satch_it;
    
    _server_blocks = _parsing.get_virtual_servers();
    satch_it = _server_blocks.begin();

    while (satch_it != _server_blocks.end())
    {
        t_server_block block = *satch_it;

        if ((it = _servers.find(block.port)) == _servers.end())
        {
            hosts_map new_host;
            server_names_map new_name;
            block.is_default = true;

            new_name.insert(std::make_pair(block.server_name, block));
            new_host.insert(std::make_pair(block.host, new_name));
            _servers.insert(std::make_pair(block.port, new_host));
        }
        else
        {
            if ((itt = it->second.find(block.host)) == it->second.end())
            {
                server_names_map new_name;
                block.is_default = true;
                new_name.insert(std::make_pair(block.server_name, block));
                it->second.insert(std::make_pair(block.host, new_name));
            }
            else
            {
                if ((ittt = itt->second.find(block.server_name)) == itt->second.end())
                {
                    itt->second.insert(std::make_pair(block.server_name, block));
                }
                else
                    perror("server block conflict");
            }
        }
        satch_it++;
    }
}

/**
 * @brief Destroy the Webserver:: Webserver object
 */
Webserver::~Webserver()
{
    while (_sockets.size())
    {
        close(_sockets.back().fd);
        _sockets.pop_back();
    }
}

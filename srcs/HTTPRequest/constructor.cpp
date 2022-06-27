/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 02:29:52 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 14:39:39 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

/**
 * @brief Construct a new HTTPRequest::HTTPRequest object
 * 
 * @param first_line vector<string> of request
 * @param server    - string [ IP : port ]
 * @param client    - string [ IP : port ]
 */

HTTPRequest::HTTPRequest(std::istringstream &request, servers_map &servers_map, int socket_fd) :
_flags(0),
_socket_fd(socket_fd)
{
    // Parse buffer
    _return_code = http_tools::parse_incoming_buffer(request, _headers, true);
    // LOG AND DEBUG
    std::cout << YELLOW "parse return code : " << BLUE <<_return_code << RESET << std::endl;
    if (_return_code == BAD_REQUEST)
    {
        _flags |= S_COMPLETE;
        return ;
    }
    // Set Method
    _set_type();
    // Map request to servers
    _server = get_server_block(_headers["Host"], socket_fd, servers_map);
    // Parsing URL 
    get_url_extension();
}

/**
 * @brief set type of request (?? redundant with map parsing method ??)
 */

void HTTPRequest::_set_type()
{
    std::cout << GREEN << "Method : " << RESET;
    if (_headers["Method"] == "GET")
    {
        _flags |= M_GET;
        std::cout << CYAN << "GET" << RESET << std::endl;
    }
    if (_headers["Method"] == "POST")
    {
        _flags |= M_POST;
        std::cout << CYAN << "POST" << RESET << std::endl;
    }
    if (_headers["Method"] == "DELETE")
    {    
        _flags |= M_DELETE;
        std::cout << CYAN << "DELETE" << RESET << std::endl;
    }
}
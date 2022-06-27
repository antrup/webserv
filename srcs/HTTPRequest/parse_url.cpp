/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_url.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 23:06:22 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 15:41:09 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

/**
 * @brief parse URL if CGI request is detected
 */

void HTTPRequest::parse_url(locations::iterator location)
{
    std::string client;
    std::string query;
    std::string url;
    std::string script_name;

    client = http_tools::get_client_ip(_socket_fd);
    
    // Get request referer
    if (_headers["Referer"].size())
        url = _headers["Referer"];

    // GET Request URL
    url += _headers["URL"];

    // Extract QUERY string
    size_t qpos = url.find('?');
    if (qpos != std::string::npos)
    {
        query = url.substr(qpos + 1);
        _headers.insert(header_pairs("QUERY_STRING", query));
    }

    // Extract SCRIPT NAME
    size_t pos = url.find(_headers["Host"]);
    if (pos != std::string::npos)
        pos += _headers["Host"].size();
    else
        pos = url.find('/');
    if (pos != std::string::npos)
        script_name = url.substr(pos + 1, qpos - pos - 1);
    if (script_name.size())
        _headers.insert(header_pairs("SCRIPT_NAME", script_name));
    
    // GET PATH INFO
    std::string path_info = location->root + "/" + script_name;
    _headers.insert(header_pairs("PATH_INFO", path_info));

    // Extract REMOTE HOST
    pos = client.find(':');
    if (pos != std::string::npos)
        _headers.insert(header_pairs("REMOTE_ADDR", client.substr(0, pos)));
    

}

/**
 * @brief insert requested headers
 */
void HTTPRequest::get_url_extension()
{
    // Extract extension for CGI
    std::string URI = _headers["URL"];
    // Check if we have a query
    size_t qpos = URI.find('?');
    if (qpos != std::string::npos)
        URI = URI.substr(0, qpos);
    size_t pos = URI.find('.');
    if (pos != std::string::npos)
    {
        std::string extension = URI.substr(pos, URI.size() - pos);
        _headers.insert(header_pairs("EXTENSION", extension));
    }
}
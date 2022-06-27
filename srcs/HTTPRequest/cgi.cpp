/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 23:20:09 by toni              #+#    #+#             */
/*   Updated: 2022/04/01 19:27:46by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "GET_request.hpp"
#include "POST_request.hpp"


/**
 * @brief create char **env for CGI script
 * 
 */

std::vector<std::string>  HTTPRequest::set_env()
{
    HTTPRequest::headers::iterator it;

    std::vector<std::string> tmp;

    tmp.push_back(std::string("SERVER_NAME=") + _server->server_name);
    tmp.push_back(std::string("SERVER_PORT=") + _server->port);

    tmp.push_back("GATEWAY_INTERFACE=CGI/1.1");
    tmp.push_back("SERVER_SOFTWARE=SANDT1.0");
    tmp.push_back("SERVER_PROTOCOL=HTTP/1.1");

    // QUERY_STRING
    if ((it = _headers.find("QUERY_STRING")) != _headers.end())
        tmp.push_back(std::string("QUERY_STRING=") + it->second);
    // SCRIPT_NAME
    if ((it = _headers.find("SCRIPT_NAME")) != _headers.end())
        tmp.push_back(std::string("SCRIPT_NAME=") + it->second);
    // PATH_INFO
    if ((it = _headers.find("PATH_INFO")) != _headers.end()) 
        tmp.push_back(std::string("PATH_INFO=") + it->second);
    // REMOTE_ADDR
    if ((it = _headers.find("REMOTE_ADDR")) != _headers.end())
        tmp.push_back(std::string("REMOTE_ADDR=") + it->second);
    // REQUEST_METHOD
    if ((it = _headers.find("Method")) != _headers.end())
        tmp.push_back(std::string("REQUEST_METHOD=") + it->second);
    // CONTENT_LENGHT
    if ((it = _headers.find("Content-Length")) != _headers.end())
        tmp.push_back(std::string("CONTENT_LENGHT=") + it->second);
    // CONTENT_TYPE
    if ((it = _headers.find("Content-Type")) != _headers.end())
        tmp.push_back(std::string("CONTENT_TYPE=") + it->second);
    
    return tmp;
}

std::vector<std::string> HTTPRequest::set_argv(std::string script_name)
{
    std::vector<std::string> args;

    args.push_back(script_name);
    args.push_back(_ressource);

    return args;
}
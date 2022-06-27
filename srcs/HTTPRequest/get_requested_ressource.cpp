/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_requested_ressource.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toni <toni@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 14:07:23 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 16:10:44 by toni             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

void    HTTPRequest::print_headers()
{
    // LOG - print parsed headers ( map<string,string> )
    std::cout << YELLOW << "HEADERS -- BEGIN" << RESET << std::endl;
    for (headers::iterator it = _headers.begin(); it != _headers.end(); it++)
    {
        std::cout << BLUE << it->first << RESET;
        std::cout << " -> ";
        std::cout << CYAN << it->second << RESET;
        std::cout << std::endl;
    }
    std::cout << YELLOW << "HEADERS -- END" << RESET << std::endl;
}

void    HTTPRequest::build_path_to_resource(locations::iterator location)
{
    // Build path for location of file
    if (location->location == _headers["URL"])
        _ressource = location->root + "/" + location->index_file;
    else
        _ressource = location->root + _headers["URL"];
    
    // LOG - ressource
    std::cout << BLUE << "ressource : "<< RESET;
    std::cout<< YELLOW << _ressource << RESET << std::endl;
}


void HTTPRequest::get_requested_ressource(locations::iterator location)
{
    // LOG AND DEBUG
    print_headers();
    // Convert to char *
    const char *path_to_file = _ressource.c_str();
    
    // Try to open ressource
    try
    {
        std::ifstream file(path_to_file);
        // Default reply conten
        if (file.fail() || !(location->flags & _flags))
            _return_code = http_tools::get_error_code(errno);
        
        errno = 0; // MUST REMOVE !!!! (perror causes endless error print)
        
        // Check if requested file exists
        if (file.good())
        {
            // set content to file content
            std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _content = str;
            _return_code = SUCCESS; // SUCCESS
        }
        // SET AS REQUEST COMPLETE
        _flags |= S_COMPLETE;
    }
    // ERROR
    catch (std::exception &err)
    {
        std::cout << err.what() << std::endl;
        // Approriate code ?
        _return_code = http_tools::get_error_code(errno);
        _flags |= S_COMPLETE;
    }
}
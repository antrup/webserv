/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:53:09 by toni              #+#    #+#             */
/*   Updated: 2022/03/24 22:53:26by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "Webserver.hpp"

void GET_request::action()
{
    // Check status and return if error
    if (_flags & S_COMPLETE || _return_code != SUCCESS)
        return ;

    // Find matching location block
    locations::iterator location = this->match_location();

    this->build_path_to_resource(location);

    // List DIRECTORY if flag is set and URL is a match
    if (location->flags & M_LIST_DIR && location->location == _headers["URL"])
        return list_directory_page(location);

    // DO CGI if extension is a match
    CGI_map::iterator it_cgi;
    if ((it_cgi = location->cgi_map.find(_headers["EXTENSION"])) != location->cgi_map.end())
        return this->handle_cgi_request(it_cgi, location);
    
    // Get requested ressource
    this->get_requested_ressource(location);
    
    // LOG - Return code
    std::cout << CYAN "After action -> ";
    if (_return_code > 400)
        std::cout << RED;
    else
        std::cout << GREEN;
    std::cout << "Return code : " << _return_code << RESET << std::endl;

    return ;
}

void    GET_request::handle_cgi_request(CGI_map::iterator cgi_location, locations::iterator location)
{
    // Handle CGI request --> need to complete !
    std::cout << GREEN << " -- CGI detected -- " << RESET << std::endl;
    _flags |= S_CGI;
    parse_url(location); // --> Need to improve
    // LOG AND DEBUG
    print_headers();
    // START CGI
    _script.set_variables(set_env(), set_argv(cgi_location->second));
    _script.execute();
    _flags |= S_COMPLETE;
}
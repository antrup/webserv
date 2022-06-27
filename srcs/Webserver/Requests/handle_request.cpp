/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_request.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 00:35:35 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 19:23:47 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

typedef std::vector<HTTPResponse *>                 response_list;
typedef std::pair<const int, response_list >        response_pair;

/**
 * @brief Handle request
 * 
 * @param socket_id socket we are using
 * @param request       request class
 */

void Webserver::handle_request(HTTPRequest *request, int socket_id)
{

    // LOG AND DEBUG
    std::cout << YELLOW "LAUNCH REQUEST ACTION" RESET << std::endl;
    // Launch request action
    request->action();

    // LOG AND DEBUG
    std::cout << CYAN " Check request status" RESET << std::endl;
    if (request->_flags & S_COMPLETE)
    {
        HTTPResponse *http_reply;
        if (request->_return_code != SUCCESS)
            http_reply = new HTTPResponse(request->_return_code, request->get_myblock()->error_pages);
        else
            http_reply = new HTTPResponse(request->_return_code, request->get_content(), request->get_CGI(), request->get_myblock()->error_pages);
        
        // INSERT Response into map
        outgoing_map::iterator outgoing_it = _response_list.find(_sockets[socket_id].fd);
        if (outgoing_it == _response_list.end())
        {
            // If first create response vector
            std::cout << YELLOW "First response" RESET << std::endl;
            response_list init_list(1, http_reply);
            _response_list.insert(response_pair(_sockets[socket_id].fd, init_list));
        }
        else
        {
            // Add to response waitlist
            std::cout << YELLOW "Queue response" RESET << std::endl;
            outgoing_it->second.push_back(http_reply);
        }
        if (request->_flags & S_CGI && request->_return_code == SUCCESS)
        {
            // Set response CGI status
            http_reply->_flags |= S_CGI;
            // Add to CGI waitlist (output)
            struct pollfd new_pollfd;
            memset(&new_pollfd, 0, sizeof(new_pollfd));
            new_pollfd.fd = http_reply->get_CGI_fd_out();
            new_pollfd.events = POLLIN;
            _cgi_fds.insert(std::pair<const int, HTTPResponse*>(new_pollfd.fd, http_reply));
            _sockets.push_back(new_pollfd);
        }
        // clean
        delete request;
    }
    return;
}

void Webserver::create_error_response(int return_code, int socket_id)
{   
    HTTPResponse *http_reply;
    http_reply = new HTTPResponse(return_code, error_pages());
    
     // INSERT Response into map
     outgoing_map::iterator outgoing_it = _response_list.find(_sockets[socket_id].fd);
     if (outgoing_it == _response_list.end())
     {
         // If first create response vector
         std::cout << YELLOW "First response" RESET << std::endl;
         response_list init_list(1, http_reply);
         _response_list.insert(response_pair(_sockets[socket_id].fd, init_list));
     }
     else
     {
         // Add to response waitlist
         std::cout << YELLOW "Queue response" RESET << std::endl;
         outgoing_it->second.push_back(http_reply);
     }
}


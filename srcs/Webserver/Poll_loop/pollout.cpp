/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollout.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 16:26:35 by atruphem          #+#    #+#             */
/*   Updated: 2022/05/21 17:46:05 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Handle POLLOUT event on a fd under poll supervision. Call the relevant function based on the type of fd involved.
 * 
 * @param socket_id ID of the fd for which a POLLOUT event has been detected 
 */

void    Webserver::pollout_event(int socket_id)
{ 
    outgoing_map::iterator outgoing_it;
    cgi_fds::iterator cgi_it;
    // Check sockets for responses
    if ((outgoing_it = _response_list.find(_sockets[socket_id].fd)) != _response_list.end())
    {    
        std::string reply;

        if (outgoing_it->second.size() && outgoing_it->second.front()->_flags & S_CHUNKED)
        {
            if (!(outgoing_it->second.front()->_flags & S_READY))
                return ;
            // SEND CHUNK - clean response buffer
            if (outgoing_it->second.front()->_flags & S_END)
                outgoing_it->second.front()->generate_chunk(0);
            reply = outgoing_it->second.front()->generate_reply();
            // Send data to socket
            send(_sockets[socket_id].fd, reply.c_str(), reply.size(), 0);
            // Clear Response stream
            outgoing_it->second.front()->clear_cgi_stream();
            std::cout << YELLOW "Chunk Response sent " RESET << std::endl;
        }
        // Check if responses is ready
        if (outgoing_it->second.size() && outgoing_it->second.front()->_flags & S_COMPLETE)
        { 
            // Generate Reply
            if (!(outgoing_it->second.front()->_flags & S_CHUNKED))
            {
                // LOG AND DEBUG            
                std::cout << CYAN << "FD " << _sockets[socket_id].fd << RESET;
                std::cout << YELLOW " - response ready " RESET;
                // Generate reply
                reply = outgoing_it->second.front()->generate_reply();
                // Send and print size
                send(_sockets[socket_id].fd, reply.c_str(), reply.size(), 0);
                std::cout << "size : " << reply.size() << std::endl;
                std::cout << YELLOW "Response SENT " RESET << std::endl;
            }
            // Clean response
            HTTPResponse *tmp = outgoing_it->second.front();
            if ((cgi_it = _cgi_fds.find(tmp->get_CGI_fd_out())) != _cgi_fds.end())
                _cgi_fds.erase(cgi_it);
            outgoing_it->second.erase(outgoing_it->second.begin());
            delete tmp;
            std::cout << BLUE "Response Cleaned " RESET << std::endl;
        }
        if (outgoing_it->second.size() && outgoing_it->second.front()->_flags & S_READY 
            && outgoing_it->second.front()->_flags & S_HEADER)
        {
            outgoing_it->second.front()->_return_code = 501;
            // LOG AND DEBUG            
            std::cout << CYAN << "FD " << _sockets[socket_id].fd << RESET;
            std::cout << YELLOW " - response ready " RESET;
            // Generate reply
            reply = outgoing_it->second.front()->generate_reply();
            // Send and print size
            send(_sockets[socket_id].fd, reply.c_str(), reply.size(), 0);
            std::cout << "size : " << reply.size() << std::endl;
            std::cout << YELLOW "Response SENT " RESET << std::endl;
            // Clean response
            HTTPResponse *tmp = outgoing_it->second.front();
            if ((cgi_it = _cgi_fds.find(tmp->get_CGI_fd_out())) != _cgi_fds.end())
                _cgi_fds.erase(cgi_it);
            outgoing_it->second.erase(outgoing_it->second.begin());
            delete tmp;
            std::cout << BLUE "Response Cleaned " RESET << std::endl;
        }
    }
}
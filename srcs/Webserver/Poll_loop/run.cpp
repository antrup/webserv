/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 18:05:52 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/16 13:20:45y sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

/**
 * @brief Run method for server
 * 
 * @return SUCCESS or FAILURE
 */

int Webserver::run()
{
    size_t rnds = 0;
    bool pstatus = false;

    while (1)
    {
        // check wether any fd in _sockets has something to be read on (POLLIN tag) or is ready for writing on it (POLLOUT tag) or has been closed by peer (POLLHUP tag)
        int ret = poll(&_sockets[0], _sockets.size(), 5);
        rnds++;
        // ERROR
        if (ret < 0)
        {
            // LOG
            std::cout << RED "POLL - FAILURE !!" << std::endl;
            return EXIT_FAILURE;
        }
        // NO event detected
        if (ret == 0)
        {
            // LOG
            if (!pstatus)
                std::cout << GREEN "POLL - Waiting .." RESET << std::endl;
            pstatus = true;
        }
        // Event detected
        else
        {
            for (size_t socket_id = 0; socket_id < _sockets.size(); socket_id++)
            {
                if (_sockets[socket_id].revents != 0)
                {
                    if (_sockets[socket_id].revents & POLLIN)
                        pollin_event(socket_id);

                    else if (_sockets[socket_id].revents & POLLOUT)
                        pollout_event(socket_id);
                    
                    else if (_sockets[socket_id].revents & POLLHUP || _sockets[socket_id].revents & POLLRDHUP)
                        pollhup_event(socket_id);

                    else if (_sockets[socket_id].revents & POLLERR || _sockets[socket_id].revents & POLLNVAL)
                        pollerr_event(socket_id);
                }
            }
            // LOG
            pstatus = false;
        }
        // Every 30s try to init faulty sockets again
        if (rnds == 6000)
        {
            if (!_init_sockets.empty())
                init_retry();
            rnds = 0;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * @brief Handle POLLIN event on a fd under poll supervision. Call the relevant function based on the type of fd involved.
 * 
 * @param socket_id ID of the fd for which a POLLIN event has been detected 
 */

void    Webserver::pollin_event(int socket_id)
{
    listen_sockets::iterator listen_it;
    cgi_fds::iterator cgi_it;
    incoming_map::iterator incoming_it;

    // Accept connection
    if ((listen_it = _listen_socket.find(_sockets[socket_id].fd)) != _listen_socket.end())
        on_client_connect(socket_id);

    // Check CGI Status & handle
    else if ((cgi_it = _cgi_fds.find(_sockets[socket_id].fd)) != _cgi_fds.end())
        cgi_it->second->handle_cgi_return();

    // Route data
    else if ((incoming_it = _data_incoming_list.find(_sockets[socket_id].fd)) != _data_incoming_list.end())
        route_incoming_data(socket_id);
}



/**
 * @brief Handle POLLHUP event on a fd under poll supervision. Call the relevant function for cleaning based on the type of fd involved
 * 
 * @param socket_id ID of the fd for which a POLLHUP event has been detected 
 */

void    Webserver::pollhup_event(int socket_id)
{
    std::cout << "POLLHUP " << socket_id << std::endl;
    cgi_fds::iterator cgi_it;
    outgoing_map::iterator outgoing_it;
    listen_sockets::iterator listen_it;

    if ((listen_it = _listen_socket.find(_sockets[socket_id].fd)) != _listen_socket.end())
        listen_socket_error(socket_id);
    else if ((cgi_it = _cgi_fds.find(_sockets[socket_id].fd)) != _cgi_fds.end())
    {
        // Close cgi output and set end flag on response
        for (outgoing_it = _response_list.begin(); outgoing_it != _response_list.end(); outgoing_it++)
        {
            if (outgoing_it->second.size() && outgoing_it->second.front()->get_CGI_fd_out() == _sockets[socket_id].fd)
                outgoing_it->second.front()->_flags |= S_END | S_READY;
        }
        close(_sockets[socket_id].fd);
        _cgi_fds.erase(cgi_it);
    }
    else
        data_socket_error(socket_id);
}

void    Webserver::pollerr_event(int socket_id)
{
    cgi_fds::iterator cgi_it;
    incoming_map::iterator data_it;
    outgoing_map::iterator outgoing_it;
    listen_sockets::iterator listen_it;

    if ((listen_it = _listen_socket.find(_sockets[socket_id].fd)) != _listen_socket.end())
        listen_socket_error(socket_id);
    
    else if ((data_it = _data_incoming_list.find(_sockets[socket_id].fd)) != _data_incoming_list.end())
        data_socket_error(socket_id);
}


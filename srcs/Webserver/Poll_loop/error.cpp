#include "Webserver.hpp"

/**
 * @brief Handle error on listen_socket
 */

void    Webserver::listen_socket_error(int socket_id)
{   
    listen_sockets::iterator listen_it;

    close(_sockets[socket_id].fd);
    _sockets.erase(_sockets.begin() + socket_id);
    listen_it = _listen_socket.find(_sockets[socket_id].fd);
    _init_sockets.push_back(std::make_pair(listen_it->second.first, listen_it->second.second));
    init_retry();
}

void    Webserver::data_socket_error(int socket_id)
{
    cgi_fds::iterator cgi_it;
    incoming_map::iterator incoming_it = _data_incoming_list.find(_sockets[socket_id].fd);
    outgoing_map::iterator outgoing_it;

    if (incoming_it != _data_incoming_list.end() && incoming_it->second)
    {
        if (incoming_it->second->_flags & S_CGI)
        {
            if ((cgi_it = _cgi_fds.find(incoming_it->second->get_CGI().get_fd_out())) != _cgi_fds.end())
                _cgi_fds.erase(cgi_it);
            kill(incoming_it->second->get_CGI()._pid, SIGKILL);
            close(incoming_it->second->get_CGI().get_fd_out());
        }
        if (incoming_it->second->_flags & M_POST)
        {
            close(static_cast<POST_request*>(incoming_it->second)->get_target_fd());
        }
        delete(incoming_it->second);
    }
    
    if ((outgoing_it = _response_list.find(_sockets[socket_id].fd)) != _response_list.end())
    {
        std::vector<HTTPResponse*>::iterator responses_it;
        responses_it = outgoing_it->second.begin();

        while (responses_it != outgoing_it->second.end())
        {
            if ((*responses_it)->get_CGI()._on)
            {
                if ((cgi_it = _cgi_fds.find((*responses_it)->get_CGI().get_fd_out())) != _cgi_fds.end())
                    _cgi_fds.erase(cgi_it);
                kill((*responses_it)->get_CGI()._pid, SIGKILL);
                close((*responses_it)->get_CGI().get_fd_out());
            }
            delete(*responses_it);
            responses_it++;
        }
        _response_list.erase(outgoing_it);
    }
    close_data_socket(socket_id);
}


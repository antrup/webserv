/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserver.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 16:05:51 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 20:45:55 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_CLASS_HPP
#define WEBSERVER_CLASS_HPP

#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "Parsing.hpp"
#include "webserv.hpp"

/**
 * @brief Webserver class
 *  Handles the initilization and creation of a HTTP/TCP server
 * 
 */
class Webserver
{
public:
    /* TYPEDEFS */
    // Sockets
    typedef std::vector<std::pair<std::string,std::string> >            to_init_sockets;    // Array of listen sockets to be init
    typedef std::map<const int, std::pair<std::string,std::string> >    listen_sockets;     // Array of listen sockets
    typedef std::vector<struct pollfd>                                  poll_sockets;       // Array of poll sockets
    // Requests
    typedef std::map<const int, HTTPResponse*>              cgi_fds;            // Array of fds used for cgi scripts output
    typedef std::map<const int, HTTPRequest*>               incoming_map;       // map of data sockets and associated requests
    // Responses
    typedef std::map<const int, std::vector<HTTPResponse*> >    outgoing_map;          // map of data sockets and associated responses

//private:
public:
    /* VARIABLES */
    servers_map         _servers;               // server blocks arranged by port, host and server_name
    to_init_sockets     _init_sockets;          // sockets to be init
    poll_sockets        _sockets;               // pollable FDs
    listen_sockets      _listen_socket;         // Listen sockets
    cgi_fds             _cgi_fds;               // fds used for cgi scripts output (mapped with their respective HTTPRequest)
    incoming_map        _data_incoming_list;   // map of data sockets and associated requests
    outgoing_map        _response_list;         // map of data sockets and associated requests
    std::vector<t_server_block> _server_blocks;
    std::vector<struct addrinfo *> socket_addr_info;
    // Parsing
    Parsing             _parsing; 

public:
    // Constructor
    Webserver(std::string config);
    // Methods
    void    init();
    int     run();
    // Destructor
    ~Webserver();
    
    void        init_retry();                                           // Retry creation of listen sockets

protected:
    // Handle request
    void        handle_request(HTTPRequest *req, int socket_id);
    //  Handle sockets
    void        init_socket(std::string port, std::string host);        // Initialize socket
    void        on_client_connect(int socket_id);                       // accept connection and add socket
    void        on_client_disconnect(int socket_id);                    // close FD and clean
    void        close_data_socket(int socket_id);                       // Close and clean disconnected data socket
    void        check_timeout();                                        // Check wether a request has timeout
    //  Route Data
    void        route_incoming_data(int socket_id);                         // Route incomming data to the relevant function
    std::string sneak_peek(int socket_id);                                  // Peek to incomming data
    void        initial_peek(int socket_id);                                // handle peek for new request's data
    int         initial_peek_post(size_t &end_of_body_pos, size_t &request_size, size_t headers_end_pos, std::string &str_buffer);
    void        additionnal_peek(int socket_id, POST_request *request);       // handle peek to incomming data relating to an existing request's body
    void        get_additionnal_data(int socket_id, size_t data_size, POST_request *request); // Get and transfer incoming data to existing HTTP request
    int         read_chunks(std::string buffer, size_t header_end);
    void        handle_bad_request(int socket_id, int return_code);                          // Handle bad request

    // Handle Request
    void        create_request(int socket_id, size_t request_size, int type);   // Read incoming request
    bool        parse_first_line(headers &ref, std::string line);       // Parse first line of request header
    void        create_error_response(int return_code, int socket_id);
    // POLL helpers
    void        pollin_event(int socket_id);
    void        pollout_event(int socket_id);
    void        pollhup_event(int socket_id);
    void        pollerr_event(int socket_id);

    // Errors handler
    void        listen_socket_error(int socket_id);
    void        data_socket_error(int socket_id);
};

#endif  // WEBSERVER_HPP
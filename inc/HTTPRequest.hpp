/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:40:17 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/21 14:35:56 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "GET_request.hpp"
#include "POST_request.hpp"
#include "DELETE_request.hpp"

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "CGI.hpp"

/**
 * @brief HTTP Request - Interface class to communicate via TCP
 */
class HTTPRequest
{
    public:
        // TYPEDEFS
        typedef std::pair<std::string, std::string> header_pairs;
        typedef std::map<std::string, std::string>  headers;

    /* VARIABLES */
    public:
        int_fast16_t    _flags;                 // flags (type / status)
        int             _return_code;           // HTTP 1.1 reply code

    protected:
        int             _socket_fd;             // socket FD 
        CGI             _script;                // CGI object
        std::string     _ressource;             // requested ressource ( ex: /tmp/toto/tata.html )
        t_server_block* _server;                // pointer to server block
        headers         _headers;               // Map of HEADER fields and content
        
        // content generated when handling request (POST / GET / DELETE)
        std::string _content;

    public:
        /* Interface methods */
        virtual void action(void) = 0;          // Execute TCP Request
        // Constructor
        HTTPRequest(std::istringstream &request_data, servers_map &s_map, int socket_fd);
        
        // GETTERS
        void            get_requested_ressource(locations::iterator location);
        const CGI       &get_CGI() const;
        std::string     get_content() const;
        t_server_block* get_myblock() const;
        // Destructor
        virtual ~HTTPRequest() {}

    protected:
        // Server block matching
        locations::iterator match_location();
        t_server_block*     get_server_block(std::string name, int fd, servers_map &servers_map);
        void                build_path_to_resource(locations::iterator location);
        // Parsing URL & headers
        void                get_url_extension();
        void                parse_url(locations::iterator location);
        // CGI related methods
        std::vector<std::string> set_env(void);
        std::vector<std::string> set_argv(std::string script_name);
        // Helper methods
        void                _set_type();
        void                chunks_decoding(std::istringstream &data);
        // 
        std::string         get_response_header();
        // LOG AND DEBUG
        void    print_headers();
};

#endif  // HTTPREQUEST_HPP
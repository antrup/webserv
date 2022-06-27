/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:40:17 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 19:24:31 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#ifndef HTTPResponse_HPP
#define HTTPResponse_HPP

#include "CGI.hpp"


/**
 * @brief HTTP Response - Class dedicated to the response workflow
 */
class HTTPResponse
{
    protected:
        // CGI
        CGI             _script;        // CGI object    
        headers         _cgi_headers;   // headers provided by the CGI script (map)
        // Content body
        ssize_t          _body_lenght;   // total size of the response body
        std::string     _body;          // body content
        // Error pages
        error_pages     _error_map;     // personalized error pages

    public:
        // Variables
        int                 _return_code;   // HTTP 1.1 return code
        int_fast16_t        _flags;         // flags (status)
        std::stringstream   _content;       // CGI output buffer

    public:
        // Constructor
        HTTPResponse(int return_code, std::string content, CGI script, error_pages error_map);
        HTTPResponse(int return_code, error_pages error_map);
        // CGI related methods
        void            handle_cgi_return();
        void            handle_cgi_location(std::string location);

        void            print_headers();

        void            handle_cgi_headers();
        void            get_response_body(std::string content_lenght);
        void            handle_chunked_body();
        void            generate_chunk(int end);
        void            clear_cgi_stream();
        
        // Reply
        std::string     generate_reply();
        std::string     generate_error_page();
        std::string     get_response_header();
        // Getters
        int             get_CGI_fd_out() const;
        const CGI       &get_CGI() const;

        // Destructor
        ~HTTPResponse() {}
};

#endif  // HTTPRESPONSE_HPP
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   POST_request.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toni <toni@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 06:14:22 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/21 10:09:18 by toni             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_REQUEST_HPP
#define POST_REQUEST_HPP

#include "HTTPRequest.hpp"

/**
 * @brief POST TCP - Allows server to respond to incoming POST requests
 */
class POST_request : public HTTPRequest
{
    private:
        std::string     _request_body;
        size_t          _body_missing_size;     // Size of body data to be received
        int             _target_fd;        // fd of CGI or file
    
    public:
        bool            _is_chunked;       // Body of the request type

        /* Interface Method */
        virtual void action();
        
        // Constructor
        POST_request(std::istringstream &request_data, servers_map &s_map, int socket_fd);
        
        /* Methods */
        void        send_to_file(std::istringstream &data) {(void)data;}    // send data (body) to file - to be done
        std::string get_request_body();
        size_t      get_body_missing_size() const;
        void        get_body(std::istringstream &data);
        void        push_data_to_fd();
        int         get_target_fd() const;
        
        // Destructor
        ~POST_request() {}

    private:
        void            is_chunked_check(void);                 // check if the request is a chunked one and set _is_chunked accordingly
        void            handle_cgi_request(CGI_map::iterator cgi_location, locations::iterator location);
};



#endif  // POST_REQUEST_HPP
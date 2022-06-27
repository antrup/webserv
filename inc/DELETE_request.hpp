/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DELETE_request.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 06:15:12 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 19:43:07 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_REQUEST_HPP
#define DELETE_REQUEST_HPP

#include "HTTPRequest.hpp"

/**
 * @brief DELETE TCP - Allows server to respond to incoming DELETE requests
 */
class DELETE_request : public HTTPRequest
{
    public:
        DELETE_request(std::istringstream &request_data, servers_map &s_map, int socket_fd) : HTTPRequest(request_data, s_map, socket_fd) {}
        ~DELETE_request() {}

        virtual void action();
};

#endif  // DELETE_REQUEST_HPP
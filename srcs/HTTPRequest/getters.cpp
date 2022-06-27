/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 16:34:25 by toni              #+#    #+#             */
/*   Updated: 2022/05/21 14:36:23 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

const CGI    &HTTPRequest::get_CGI() const
{
    return (_script);
}

std::string HTTPRequest::get_content() const
{
    return _content;
}

t_server_block* HTTPRequest::get_myblock() const
{
    return _server;
}
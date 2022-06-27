/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 16:34:25 by toni              #+#    #+#             */
/*   Updated: 2022/05/20 19:25:02 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

int     HTTPResponse::get_CGI_fd_out() const
{
    return (_script.get_fd_out());
}

const CGI    &HTTPResponse::get_CGI() const
{
    return (_script);
}

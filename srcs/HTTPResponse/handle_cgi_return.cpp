/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cgi_return.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 13:27:24 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/21 16:43:11 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

void HTTPResponse::handle_cgi_return()
{
    // LOG AND DEBUG
    std::cout << YELLOW "Handle CGI return" RESET << std::endl;

    // receive data
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int32_t end = read(get_CGI_fd_out(), buffer, BUFFER_SIZE);
    if (end == -1)
    {
        // ERROR
        _return_code = INTERNAL_ERROR;
        _flags = S_COMPLETE;
        return ;
    }
    // create stream from buffer
    std::istringstream tstream(buffer);
    _content << tstream.str();
    
    // Handle CGI headers
    std::cout << "flags : " << _flags << std::endl;
    if (_flags & S_CGI && _flags & S_HEADER)
        handle_cgi_headers();    
    if (_flags & S_COMPLETE || _flags & S_HEADER)
        return ;

    // LOG AND DEBUG
    print_headers();

    headers::iterator it;
    // check if location 
    if ((it = _cgi_headers.find("Location")) != _cgi_headers.end())
        return handle_cgi_location(it->second);
    // check if content length
    if ((it = _cgi_headers.find("Content-Lenght")) != _cgi_headers.end())
        return get_response_body(it->second);
    // Handle chunks
    if (!(_flags & S_CHUNKED))
        return handle_chunked_body();
    else
        return generate_chunk(end);
    
    std::cout << YELLOW "end handle cgi return" RESET << std::endl;
    return;
}
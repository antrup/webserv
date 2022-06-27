/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_chunks.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 19:00:33 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 20:58:43 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"

int Webserver::read_chunks(std::string buffer, size_t header_end)
{
    std::istringstream body(buffer);
    std::stringstream ss;
    std::string tmp;

    body.seekg(header_end + 4);
    unsigned int    total_size = header_end + 4;
    unsigned int    chunk_size;
    unsigned int    total_body = 0;

    while (getline(body, tmp, '\n'))
    {
        // Extract chunk size as string
        std::string size = tmp.substr(0, tmp.find('\r'));
        // convert to uint
        ss << std::hex << size;
        ss >> chunk_size;
        // clear stream
        ss.clear();
        // get total size
        total_size += chunk_size + tmp.size() + 1;
        total_body += chunk_size;
        // place stream at correct position for getline
        body.seekg(total_size);
        if (body.fail())
            return 0;
    }
    if (total_body > _parsing._max_body_size)
        return -1;
    return total_size;
}
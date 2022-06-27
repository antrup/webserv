/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 23:23:23 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/16 14:33:16 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#define CLASSIC_R 4
#define INVALID_R 5

namespace test_request
{
std::string invalid_get()
{
    std::ostringstream stream;

    std::string invalid[INVALID_R] =
    {
        "localhost\r\n\r\n",
        "HTTP/1.1\r\n\r\nHost: localhost\r\n",
        "/iawdaphp HTTP/1.0\r\nHost: localhost\r\n",
        "       ",
        "\n"
    };

    /* Method */
    stream << "GET ";
    /* TEST */
    stream << invalid[(std::rand()/(RAND_MAX + 1u)) % INVALID_R];
    
    return (stream.str());
}

static unsigned int test = 0;

std::string classic_get()
{
    std::ostringstream stream;
    
    std::string examples[CLASSIC_R] =
    {
        "/",
        "/home",
        "/index.html",
        "/bad_shit"
    };

    /* Method */
    stream << "GET ";
    /* TEST */
    stream << examples[test];
    /* HTTP version */
    stream << " HTTP/1.1";
    /* END START LINE */
    stream << "\r\n";
    /* Host header */
    stream << "Host: localhost";
    /* END HEADERS*/
    stream << "\r\n\r\n";

    test++;
    if (test == CLASSIC_R)
        test = 0;
    return (stream.str());
}

}   // END NAMESPACE
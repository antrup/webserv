/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_request.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 18:12:44 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/14 14:46:47 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

#define SEED 42
#define TEST_COUNT 4

#ifndef _TEST_CASE
#define _TEST_CASE
# define RDM_REQUEST -1
# define GET_CLASSIC 0
# define GET_INVALID 1
# define POST_CHUNCKED 2
# define POST_CLASSIC 3
#endif

std::string HTTPClient::generate_request(int type)
{
    std::srand(SEED);
    
    std::string (*test_requests[TEST_COUNT])() =
    {
        &test_request::classic_get,
        &test_request::invalid_get,
        &test_request::chuncked_post,
        &test_request::classic_post
    };
    if (type == RDM_REQUEST)
        return (test_requests[std::rand() % TEST_COUNT]());
    else
        return test_requests[type]();
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 14:18:42 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/14 15:23:25y sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <stdlib.h>

void get_test_conditions(char **argv, int *is_slow, int *type)
{
    *type = atoi(argv[1]);
    std::cout << BLUE "TYPE : " RESET <<  *type << std::endl;
    if (*type > 3)
    {
        std::cout << RED "Invalid type" RESET << std::endl;
        exit(1);
    }
    *is_slow = atoi(argv[2]);
    std::cout << BLUE "AMOUNT : " RESET <<  *is_slow << std::endl;
    if (*is_slow != 0 && *is_slow != 1)
    {
        std::cout << RED "Invalid is_slow variable" RESET << std::endl;
        exit(1);        
    }
    
}

void client_usage(void)
{
    std::cout << BLUE " -- TEST CLIENT USAGE -- " RESET << std::endl;
    std::cout << std::endl;
    std::cout << YELLOW "Run without args to run RANDOM tests : " RESET;
    std::cout << "./test_client" << std::endl;
    std::cout << std::endl;
    std::cout << YELLOW "Use args to run specific tests" RESET << std::endl;
    std::cout << "./test_client " YELLOW "[ Request Type ] [ send slowly (1 or 0) ]" RESET << std::endl;
    std::cout << std::endl;
    std::cout << CYAN "Available types : 0 - 1 - 2 - 3" RESET << std::endl;
    std::cout << "0 - classic get" << std::endl;
    std::cout << "1 - invalid get" << std::endl;
    std::cout << "2 - post chunked" << std::endl;
    std::cout << "3 - post classic" << std::endl;
    exit(1);
}

#define RDM_REQUEST -1
#define GET_CLASSIC 0
#define GET_INVALID 1
#define POST_CHUNCKED 2
#define POST_CLASSIC 3


int main(int argc, char **argv)
{
    // Available user inputs
    int is_slow;
    int request_type;

    // USAGE
    if (argc != 3 && argc != 1)
        client_usage();
    
    // parse user inputs
    if (argc == 3)
        get_test_conditions(argv, &is_slow, &request_type);
    else
    {
        // DEFAULTS
        is_slow = -1; 
        request_type = RDM_REQUEST;
    }

    HTTPClient client;

    // Client
    client.init();
    client.connect();
    client.run(request_type, is_slow);
    client.disconnect();
   
}
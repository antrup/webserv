/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 14:36:22 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 17:40:00 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_TEST_CLIENT_HPP
#define HTTP_TEST_CLIENT_HPP

// Standard Libraries
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <sstream>

// SOCKET IO Libraries
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

// LOG colors
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define GREY "\033[0;37m"
#define RESET "\033[m"

typedef struct sockaddr SA; // used in connect.cpp

/**
 * @brief client ID to store data
 */
typedef struct test_client_id
{
    int                 socket_fd;          // FD of socket
    struct sockaddr_in  server_address;     // socket address IN
    std::string         IP_address;         // IP address
    std::string         host_name;          // Host: host_name (Header)
    unsigned int        port;               // port to connect to
} client_id; 


#define MAX_CONNECTIONS 3       // number of ports to connect to
#define MAX_BUFFER 640000       // size of stack allocated msg buffer

namespace test_request
{
    std::string invalid_get();
    std::string classic_get();
    std::string chuncked_post();
    std::string classic_post();
}

/**
 * @brief HTTP Client to test Webserv - Generate HTTP Requests
 */

class HTTPClient
{
    // Variables
    private:
        std::vector<client_id>      connections;
        std::vector<struct pollfd>  poll_sockets;

    // Methods
    public:
        void init();
        void connect();
        void run(int type, int is_slow);
        void disconnect();
    
    //  RUN - Helpers
    private:
        void handle_outgoing(int fd, int type, bool slow);
        void handle_incoming(int fd);
        std::string generate_request(int type);

    // DEFAULT
    public:
        HTTPClient();
        ~HTTPClient();
};

#endif // HTTP_TEST_CLIENT
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 18:31:27 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 15:27:48 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserver.hpp"
#include <signal.h>

Webserver *g_server;

void clean_exit(int sig_id)
{
    if (sig_id == SIGINT)
    {
        std::cout << GREEN "Clean Sockets" RESET << std::endl;
        while (g_server->_sockets.size())
        {
            close(g_server->_sockets.back().fd);
            g_server->_sockets.pop_back();
        }
        std::cout << GREEN "Clean Incoming Requests" RESET << std::endl;
        while (g_server->_data_incoming_list.size())
        {
            delete g_server->_data_incoming_list.begin()->second;
            g_server->_data_incoming_list.erase(g_server->_data_incoming_list.begin());
        }
        std::cout << GREEN "Clean Outgoing Response" RESET << std::endl;
        while (g_server->_response_list.size())
        {
            while (g_server->_response_list.begin()->second.size())
            {
                delete *(g_server->_response_list.begin()->second.begin());
                g_server->_response_list.begin()->second.erase(g_server->_response_list.begin()->second.begin());
            }
            g_server->_response_list.erase(g_server->_response_list.begin());
        }
        std::cout << GREEN "Clean addrinfo" RESET << std::endl;
        while (g_server->socket_addr_info.size())
        {
            freeaddrinfo(g_server->socket_addr_info.back());
            g_server->socket_addr_info.pop_back();
        }
        std::cout << GREEN "Exit" RESET << std::endl;
        exit(0);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage : ./webserv [ config_file ]" << std::endl;
        exit(1);
    }
    std::string config(argv[1]);
    try 
    {
        Webserver server(config);
        g_server = &server;
        struct sigaction SIGINT_handler;
        SIGINT_handler.sa_flags = SA_SIGINFO;
        SIGINT_handler.sa_handler = &clean_exit;
        sigaction(SIGINT, &SIGINT_handler, 0);
        try 
        {
            server.init();
            if (server._listen_socket.empty() && server._init_sockets.empty())
            {
                std::cout << "Fatal error, socket configuration invalid" << std::endl;
                return 1;
            }
            else if (server._listen_socket.empty())
            {
                sleep(30);
                server.init_retry();
            }
            server.run();
        }
        catch (std::exception &err)
        {
            std::cerr << err.what() << std::endl;
        }
    }
    catch (std::exception &err)
    {
        std::cerr << err.what() << std::endl;
    }
    return 0;
}
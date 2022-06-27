/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 17:54:21 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 17:44:45 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void print_log(const char *message, std::string direction)
{
    std::cout << CYAN "-- " << direction << " start --" RESET << std::endl;
    std::cout << message << std::endl;
    std::cout << CYAN " -- " << direction << " end -- " RESET << std::endl;    
}

void HTTPClient::handle_incoming(int fd)
{
    char buff[MAX_BUFFER];
    size_t  ret;

    std::cout << YELLOW "POLLIN detected" RESET << std::endl;
    // RECEIVE
    ret = recv(fd, buff, MAX_BUFFER, 0);
    std::cout << BLUE "message received : " RESET << ret << " bytes read" << std::endl;
    // LOG - print server reply
    print_log(buff, "response");
    // clean buffer
    bzero(buff, sizeof(buff));
}



void HTTPClient::handle_outgoing(int fd, int type, bool slow)
{
    std::string msg;

    std::cout << YELLOW "POLLOUT detected" RESET << std::endl;
    
    // GENERATE REQUEST --> to add tests
    msg = generate_request(type);  /* CHANGE CONSTANT TO REQUIRED TEST */
    
    // LOG - print outgoing message
    print_log(msg.c_str(), "request");
    
    if (slow && type > 1)
    {
        // split message
        size_t start = 0;
        std::string tmp;
        for (int i = 0; i < 4; i++)
        {
            tmp = msg.substr(start, msg.size() / 4);
            send(fd, tmp.c_str(), tmp.size(), 0);
            usleep(500000);
            start += tmp.size();
        }
        tmp = msg.substr(start);
        send(fd, tmp.c_str(), tmp.size(), 0);
    }   
    else
    {
        // SEND
        send(fd, msg.c_str(), msg.size(), 0);
    }
    std::cout << BLUE "message sent : " RESET << msg.size() << " bytes sent" << std::endl;    
}

// for current state of testing only

void HTTPClient::run(int type, int is_slow)
{
    std::cout << GREEN "RUN CLIENT" RESET << std::endl;
    bool slow;
    if (is_slow == 1)
        slow = true;
    else
        slow = false;
    while (1)
    {
        if (is_slow == -1 && std::rand() % 2)
            slow = true;       
        // NEEDED FOR CURRENT PROJECT STATE (multiple requests in one read)
        sleep(2); // --> to be removed
        if (::poll(&poll_sockets[0], poll_sockets.size(), 10))
        {
            std::cout << YELLOW "poll revent detected" RESET << std::endl;
            for (int i = 0; i < MAX_CONNECTIONS; i++)
            {
                std::cout << CYAN " socket FD " << poll_sockets[i].fd << RESET << std::endl;
                // INCOMING MESSAGE
                if (poll_sockets[i].revents & POLLIN)
                    handle_incoming(poll_sockets[i].fd);
                // OUTGOING MESSAGE
                else if (poll_sockets[i].revents & POLLOUT)
                    handle_outgoing(poll_sockets[i].fd, type, slow);
                // HANDLE DISCONNECT
                else if (poll_sockets[i].revents & POLLHUP)
                    std::cout << "POLLHUP detected" << std::endl;
            }
        }
    }
}
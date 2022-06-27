/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 15:24:24 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 18:00:26 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

// Standard headers
#include <cstring>
#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <algorithm>
// C Lib
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
// Containers
#include <map>
#include <vector>
#include <string>
#include <iterator>
// TCP headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

// POLL CONSTANTS
#define MAX_BACKLOG 50
#define POLL_TIMEOUT 1000 
#define BUFFER_SIZE 65536 

// LOG AND DEBUG
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define GREY "\033[0;37m"
#define RESET "\033[m"

// HTML constants
#define HEADER_END "\r\n\r\n"
#define CHUNK_END "\r\n0\r\n\r\n"
#define CHUNK_END_SIZE 7
// HTTP STD RETURN CODES
#include "return_codes.hpp"

/* FLAGS */
/* Methods */
#define M_POST (1 << 0)
#define M_GET (1 << 1)
#define M_DELETE (1 << 2)
#define M_LIST_DIR (1 << 3)
/* Request Status */
#define S_HEADER (1 << 4)   // Incomplete header
#define S_BODY (1 << 5)     // Incomplete body
#define S_CGI (1 << 6)      // CGI Processing
#define S_COMPLETE (1 << 7) // Complete / Incomplete flag
/* HTTPResponse */
#define S_READY (1 << 8)            // Ready / Not ready flag
#define S_REPLY_COMPLETE (1 << 9)   // Reply complete / incomplete flag
/* BODY FLAGS ? (ENCODING !!) */
#define S_CHUNKED (1 << 10)
#define S_END (1 << 11)

#define HEADER_END "\r\n\r\n"

// Server Map (Virtual Hosts)
typedef std::map<std::string, struct t_server_block>    server_names_map;   // map of server_names - server_blocks
typedef std::map<std::string, server_names_map>         hosts_map;          // map of host_name - server_names map
typedef std::map<std::string, hosts_map>                servers_map;        // server_map
// MULTI CGI LOCATION MAP
typedef std::map<std::string, std::string>      CGI_map;
// ERROR CODE - ERROR NAME
typedef std::map<int, std::string>              error_pages;
// HEADER FIELD - HEADER CONTENT
typedef std::map<std::string, std::string>      headers;

typedef std::vector<struct t_location_block>    locations;

/* Location Blocks */
typedef struct t_location_block
{
    std::string     location;   //the requested file/location
    std::string     root;       //the location on the server
    char            flags;      // accept requests (GET|POST|DELETE)
    std::string     index_file; // default index file
    CGI_map         cgi_map;    // file type (extension) -- cgi_pass
    bool            accept_upload; // Make the route able to accept uploaded files
    std::string     upload_dir; // Upload dir
}   t_location_block;

/* Server Blocks */
typedef struct t_server_block
{
    bool            is_default;    // by satcheen
    std::string     host;          // the hostname
    std::string     port;          // the port we have to listen
    std::string     server_name;   // the server name 
    error_pages     error_pages;    // default error pages
    size_t          body_limit;     // max body size in bytes
    locations       locations;      // list of location blocks
}   t_server_block;
    
/** TOOLS - common to several classes **/

namespace http_tools
{
    bool                parse_start_line(std::istringstream &buffer, headers &headers);
    int                 parse_incoming_buffer(std::istringstream &buffer, headers &headers, bool start_line);
    std::string         generate_html_header();
    struct sockaddr_in  get_ipv4_addr(int fd);
    std::string         get_host(struct sockaddr_in *addr_v4);
    std::string         get_port(struct sockaddr_in *addr_v4);
    int                 get_error_code(int error_no);
    std::string         get_client_ip(int fd);
}

#endif  // WEBSERVER_HPP
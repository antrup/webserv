/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 23:20:09 by toni              #+#    #+#             */
/*   Updated: 2022/04/01 19:27:46by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"
#include "HTTPRequest.hpp"


void    HTTPResponse::print_headers()
{
    // LOG - print parsed headers ( map<string,string> )
    std::cout << YELLOW << "HEADERS -- BEGIN" << RESET << std::endl;
    for (headers::iterator it = _cgi_headers.begin(); it != _cgi_headers.end(); it++)
    {
        std::cout << BLUE << it->first << RESET;
        std::cout << " -> ";
        std::cout << CYAN << it->second << RESET;
        std::cout << std::endl;
    }
    std::cout << YELLOW << "HEADERS -- END" << RESET << std::endl;
}
/**
 * @brief get response body 
 */
void HTTPResponse::get_response_body(std::string content_lenght)
{
    std::cout << YELLOW "Handle CGI body" RESET << std::endl;
    
    // get Content-lenght data
    std::stringstream ss(content_lenght);
    ss >> _body_lenght;
    
    // read _body_lenght bytes
    size_t hend = _content.str().find("\r\n\r\n");
    
    _body = _content.str().substr(hend);
    _flags |= S_READY | S_COMPLETE;
}
/**
 * @brief get chunked body
 */
void HTTPResponse::handle_chunked_body()
{
    std::cout << YELLOW "Handle CGI chunked" RESET << std::endl;
    
    size_t hend = _content.str().find("\r\n\r\n");

    std::stringstream ss;
    std::string tmp;
    size_t chunk_size; 

    tmp = _content.str().substr(hend + 4);
    chunk_size = tmp.size();

    ss << std::hex << chunk_size;
    ss << "\r\n";
    ss << tmp;
    ss << "\r\n";

    // set chunk body
    _body = ss.str();
    // set flags
    _flags |= S_CHUNKED | S_READY;
    std::cout << _body << std::endl;
}

void HTTPResponse::generate_chunk(int end)
{    
    // create stream from buffer
    std::stringstream ss;
    size_t chunk_size;

    if (end == 0)
    {
        //_flags &= ~(S_CHUNKED);
        _flags |= S_COMPLETE;
        _flags |= S_READY;
        ss << "0\r\n\r\n";
        _body = ss.str();
        return ;
    }

    chunk_size = end;

    ss << std::hex << chunk_size;
    ss << "\r\n";
    ss << _content.str();
    ss << "\r\n";

    // set chunk body
    _body = ss.str();
    _flags |= S_READY;
    // LOG AND DEBUG
    std::cout << BLUE " Generate Chunk " RESET << std::endl;
    std::cout << _body << std::endl;
}

void HTTPResponse::clear_cgi_stream()
{
    _content.str("");
    _content.clear();
    _body.clear();
    _flags &= ~(S_READY);
}
/**
 * @brief get CGI headers
 */
void HTTPResponse::handle_cgi_headers()
{
    std::cout << YELLOW "Handle CGI headers" RESET << std::endl;
    
    // check if header end
    if (_content.str().find("\r\n\r\n") == std::string::npos)
    {
        _flags |= S_HEADER;
        return ;
    }

    // convert to istringstream
    std::istringstream temp(_content.str());

    // get and parse header
    if (http_tools::parse_incoming_buffer(temp, _cgi_headers, false) == BAD_REQUEST)
    {
        _return_code = BAD_REQUEST;
        _flags |= S_READY;
        return ;
    }
    _flags &= ~(S_HEADER);
}
/**
 * @brief handle CGI location 
 */
void HTTPResponse::handle_cgi_location(std::string location)
{
    std::cout << YELLOW "Handle CGI location" RESET << std::endl;
    const char *path_to_file = location.c_str();
    // Try to open ressource
    try
    {
        std::ifstream file(path_to_file);
        // Default reply conten
        if (file.fail())
            _return_code = http_tools::get_error_code(errno);
        
        errno = 0; // MUST REMOVE !!!! (perror causes endless error print)
        
        // Check if requested file exists
        if (file.good())
        {
            // set content to file content
            std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            _body = str;
            _return_code = SUCCESS; // SUCCESS
        }
        // SET AS REQUEST COMPLETE
        _flags |= S_READY;
    }
    // ERROR
    catch (std::exception &err)
    {
        std::cout << err.what() << std::endl;
        // Approriate code ?
        _return_code = http_tools::get_error_code(errno);
        _flags |= S_READY;
    }
    return;
}
/**
 * @brief Handle return from CGI
 */

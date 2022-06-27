/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_request.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/15 02:05:51 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 15:51:22 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

namespace http_tools
{

/**
 * @brief Parse START_LINE
 */

bool parse_start_line(std::istringstream &request, headers &headers)
{
    size_t n = 0;
    int count = 0;
    // LOG AND DEBUG
    std::string start_line;
    getline(request, start_line, '\n');
    std::cout << CYAN << "start line : " RESET << start_line << std::endl;
    // check whitespace count
    while (n < start_line.size())
    {
        if (start_line[n++] == ' ')
            count++;
    }
    // error check
    if (count != 2)
        return false;
    // split line into words
    std::istringstream tmp(start_line);
    std::vector<std::string> parse_request((std::istream_iterator<std::string>(tmp)), std::istream_iterator<std::string>());
    // push the strings into the headers map
    headers.insert(std::make_pair("Method", parse_request[0]));
    headers.insert(std::make_pair("URL", parse_request[1]));
    headers.insert(std::make_pair("HTTP", parse_request[2]));
    return (true);
}

/**
 * @brief Parse headers
 * 
 * @param buffer incoming buffer to be parsed
 * @param start_line true or false to parse start line
 * @return int - return a status code
 */

int parse_incoming_buffer(std::istringstream &buffer, headers &headers, bool start_line)
{
    if (start_line && !parse_start_line(buffer, headers))
        return BAD_REQUEST;
    std::vector<std::string> parse_request;
    std::string tmp;
    
    // Read line by line
    while (getline(buffer, tmp, '\n'))
    {
        // invalid format
        if (tmp[0] == ' ' || tmp[0] == '\t')
            return BAD_REQUEST;
        // incomplete
        if (buffer.eof())
            return BAD_REQUEST;
        // Header end
        if (tmp == "\r")
            break ;
        parse_request.insert(parse_request.end(), tmp);
    }
    // LOG AND DEBUG
    std::cout << CYAN "http_tools parse request" RESET << std::endl;
    
    std::vector<std::string>::iterator header_field;
    header_field = parse_request.begin();
    headers::iterator it;
    while (header_field != parse_request.end())
    {
        bool duplicate = false;
        if (header_field->size() == 1)
            break ;
        // check if last char is ':'
        size_t pos = header_field->find(": ");
        if (pos == std::string::npos || (*header_field)[pos - 1] == ' ' || (*header_field)[pos - 1] == '\t')
            return BAD_REQUEST;
        std::string header = header_field->substr(0, pos);
        //std::cout << header << std::endl;
        it = headers.find(header);
        if (it != headers.end())    
            duplicate = true;
        std::string header_content = header_field->substr(pos + 2, header_field->size() - pos - 3);
        //std::cout << header_content << std::endl;
        header_field++;
        if (!duplicate)
            headers.insert(make_pair(header, header_content));
        else
            it->second.append(" " + header_content);
    }
    if (headers.find("Host") == headers.end() && start_line)
        return BAD_REQUEST;
    return (SUCCESS);
}

}   // END NAMESPACE
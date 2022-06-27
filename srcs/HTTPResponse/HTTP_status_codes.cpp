/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTP_status_codes.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 09:55:50 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/16 15:19:37 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

/**
 * @brief Map of return codes - definitions (ISO)
 * 
 * @return std::string - definition based on return code
 */

std::string HTTPResponse::get_response_header()
{
    typedef std::map<const int, std::string> status_codes;
    
    status_codes response;
    // Status codes	- Meaning

    // 1xx Informational

    response[100] = "Continue";
    response[101] = "Switching protocols";
    response[102] = "Processing";
    response[103] = "Early Hints";

    // 2xx Succesful

    response[200] = "OK";
    response[201] = "Created";
    response[202] = "Accepted";
    response[204] = "Non-Authoritative Information";
    response[204] = "No Content";
    response[205] = "Reset Content";
    response[206] = "Partial Content";
    response[207] = "Multi-Status";
    response[208] = "Already Reported";
    response[226] = "IM Used";

    // 3xx Redirection	 ;

    response[300] = "Multiple Choices";
    response[301] = "Moved Permanently";
    response[302] = "Found (Previously \"Moved Temporarily\")";
    response[303] = "See Other";
    response[304] = "Not Modified";
    response[305] = "Use Proxy";
    response[306] = "Switch Proxy";
    response[307] = "Temporary Redirect";
    response[308] = "Permanent Redirect";

    // 4xx Client Error	 ;

    response[400] = "Bad Request";
    response[401] = "Unauthorized";
    response[402] = "Payment Required";
    response[403] = "Forbidden";
    response[404] = "Not Found";
    response[405] = "Method Not Allowed";
    response[406] = "Not Acceptable";
    response[407] = "Proxy Authentication Required";
    response[408] = "Request Timeout";
    response[409] = "Conflict";
    response[410] = "Gone";
    response[411] = "Length Required";
    response[412] = "Precondition Failed";
    response[413] = "Payload Too Large";
    response[414] = "URI Too Long";
    response[415] = "Unsupported Media Type";
    response[416] = "Range Not Satisfiable";
    response[417] = "Expectation Failed";
    response[418] = "I'm a Teapot";
    response[421] = "Misdirected Request";
    response[422] = "Unprocessable Entity";
    response[423] = "Locked";
    response[424] = "Failed Dependency";
    response[425] = "Too Early";
    response[426] = "Upgrade Required";
    response[428] = "Precondition Required";
    response[429] = "Too Many Requests";
    response[431] = "Request Header Fields Too Large";
    response[451] = "Unavailable For Legal Reasons";

    // 5xx Server Error	 ;

    response[500] = "Internal Server Error";
    response[501] = "Not Implemented";
    response[502] = "Bad Gateway";
    response[503] = "Service Unavailable";
    response[504] = "Gateway Timeout";
    response[505] = "HTTP Version Not Supported";
    response[506] = "Variant Also Negotiates";
    response[507] = "Insufficient Storage";
    response[508] = "Loop Detected";
    response[510] = "Not Extended";
    response[511] = "Network Authentication Required";

    return (response[_return_code]);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_pages.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/27 11:18:49 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 19:08:17 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

std::string HTTPResponse::generate_error_page()
{
    std::ostringstream oss;

    error_pages::iterator it;
    if ((it = _error_map.find(_return_code)) != _error_map.end())
    {
        const char *path_to_file = it->second.c_str();
        // Try to open ressource
        std::ifstream file(path_to_file);
        if (file.fail())
            goto Bplan;
       oss << file.rdbuf();
       file.close();
    }
    else
    {
        Bplan:
        oss << "<html>";
        oss << "<head>";
        oss << "<title>";
        oss << get_response_header();
        oss << "</title>";
        oss << "<style type=\"text/css\">";
        oss << "body {";
        oss << "font-family: \"Courier New\", Courier, monospace;";
        oss << "font-size: 16px;";
        oss << "line-height: 16px;";
        oss << "letter-spacing: -4;";
        oss << "}";
        oss << "pre {";
        oss << "display: block;";
        oss << "margin: 1em auto;";
        oss << "width: 250px;";
        oss << "}";
        oss << "h1 {";
        oss << "display: block;";
        oss << "margin: 1em auto;";
        oss << "text-align: center;";
        oss << "}";
        oss << "</style>";
        oss << "</head>";
        oss << "<body>";
        oss << "<h1>";
        oss << _return_code;
        oss << "</h1>";
        oss << "<pre>";
        oss << "....................../''')\n";
        oss << "....................,/.../ \n";
        oss << ".................../..../ \n";
        oss << "............./'''/'...'/''''-, \n";
        oss << "........../'/.../..../....../''\\ \n";
        oss << "........('(...'...'.... '~/'...') \n";
        oss << ".........\\.................'.../ \n";
        oss << "..........''...\\.......... _.'' \n";
        oss << "............\\..............( \n";
        oss << "..............\\.............\\...\n";
        oss << "</pre>";
        oss << "<h1>" << get_response_header() << "</h1>";
        oss << "</body>";
        oss << "</html>";
    }
    return oss.str();
}
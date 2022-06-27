/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_directory_page.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 11:50:38 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/19 15:45:37 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "Webserver.hpp"
#include <dirent.h>

void    GET_request::list_directory_page(locations::iterator location)
{
    // LOG AND DEBUG
    this->print_headers();
    // flag request
    _flags |= S_COMPLETE;
    
    std::string path = "./" + location->root;
    DIR *dir;
    struct dirent *diread;
    std::vector<char *> files;
    // Open directory and read content
    if ((dir = opendir(path.c_str())) != NULL)
    {
        while ((diread = readdir(dir)) != NULL)
            files.push_back(diread->d_name);
    }
    else
    {
        // Directory open failure
        std::cout << RED "Failed to open directory " RESET << path << std::endl;
        _return_code = http_tools::get_error_code(errno);
        return ;
    }
    // SUCCESS
    _return_code = SUCCESS;
    
    std::sort(files.begin(), files.end());
    // Generate html directory listing
    std::ostringstream oss;
    oss << http_tools::generate_html_header();
    oss << "<ul>\n";
    for (size_t i = 0; i < files.size(); i++)
    {
        oss << "<li>";
        oss << "<a href=\"" << files[i] << "\">"; 
        oss << files[i];
        oss << "</a>";
        oss << "</li>\n";
    }
    oss << "</ul>\n";
    oss << "</body>\n";
    oss << "</html>";
    closedir(dir);
    // Set request content
    this->_content = oss.str();
}
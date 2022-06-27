/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_method.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 15:53:09 by toni              #+#    #+#             */
/*   Updated: 2022/03/24 22:53:26by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"
#include "Webserver.hpp"
#include <dirent.h>
#include <errno.h>


void POST_request::action()
{
    // Check status and return if error
    //if (_flags & S_COMPLETE || _return_code != SUCCESS)
      //  return ;

    // Find matching location block
    locations::iterator location = this->match_location();

    // get path to ressource
    this->build_path_to_resource(location);
    
    // DO CGI if extension is a match
    CGI_map::iterator it_cgi;
    if ((it_cgi = location->cgi_map.find(_headers["EXTENSION"])) != location->cgi_map.end())
        return this->handle_cgi_request(it_cgi, location);
    else // UPLOAD
    {
        if (location->accept_upload == false)
        {
            _return_code = 501;
            _flags |= S_COMPLETE;
            return;
        }
        // LOG - ressource
        std::cout << BLUE << "ressource : "<< RESET;
        std::cout<< YELLOW << _ressource << RESET << std::endl;

        // Convert to char *
        std::string complete_path =  location->upload_dir + "/" + _headers["URL"];
        const char *path_to_file = complete_path.c_str();
        _target_fd = open(path_to_file, O_CREAT|O_EXCL|O_WRONLY, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
        if (_target_fd < 0)
        {
            _return_code = http_tools::get_error_code(errno);
            _flags |= S_COMPLETE;
            return;
        }
        push_data_to_fd();
        _return_code = 200;
        _flags |= S_COMPLETE;
    }
}

void    POST_request::handle_cgi_request(CGI_map::iterator cgi_location, locations::iterator location)
{
    // Handle CGI request --> need to complete !
    std::cout << GREEN << " -- CGI detected -- " << RESET << std::endl;
    _flags |= S_CGI;
    parse_url(location); // --> Need to improve
    // LOG AND DEBUG
    print_headers();
    // START CGI
    _script.set_variables(set_env(), set_argv(cgi_location->second));
    if ((_return_code = _script.execute()) != 200)
    {
        _flags |= S_COMPLETE;
        return;
    }
    _target_fd = _script.get_fd_in();
    push_data_to_fd();
}

void    POST_request::push_data_to_fd()
{
    int ret = write(_target_fd, _request_body.c_str(), _request_body.size());
    if (ret < 0)
    {
        _return_code = 501;
        _flags |= S_COMPLETE;
        return;
    }
    _request_body.clear();
    if (_body_missing_size == 0)
        _flags |= S_COMPLETE;
}


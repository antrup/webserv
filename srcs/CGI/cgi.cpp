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

#include "CGI.hpp"

/**
 * @brief constructor
 */

CGI::CGI() :
_on(false)
{
    // LOG AND DEBUG
    std::cout << YELLOW "Init CGI object default" RESET << std::endl;
}
/**
 * @brief Construct a new CGI::CGI object from copy
 * 
 * @param copy - object to copy
 */

CGI::CGI(const CGI &copy)
{
    // LOG AND DEBUG
    std::cout << YELLOW "CGI copy constructor called" RESET << std::endl;
    _on = copy._on;
    _fd_in = copy.get_fd_in();
    _fd_out = copy.get_fd_out();
    _argv = copy.get_argv();
    _env = copy.get_env();
}

/**
 * @brief Convert vector of string to vector of char *
 * 
 * @param var_old vector of string
 * @param vars vector of char *
 */

void     CGI::convert(std::vector<std::string> &var_old, std::vector<char *> &vars)
{
    std::vector<std::string>::iterator str_it = var_old.begin();
    while (str_it != var_old.end())
    {
        vars.push_back(const_cast<char *>(str_it->c_str()));
        str_it++;
    }
    // Add null to end
    vars.push_back(NULL);
}

/**
 * @brief set env and argv
 * 
 * @param env 
 * @param argv 
 */

void    CGI::set_variables(std::vector<std::string> env, std::vector<std::string> argv)
{
    // set to object memory space
    _env = env;
    _argv = argv;
    // Convert to char *
    convert(_env, env_vars);
    convert(_argv, argv_vars);
    
    // LOG AND DEBUG
    std::cout << CYAN " -- CGI ENV -- start" RESET << std::endl;
    for (size_t i = 0; i < env_vars.size() - 1; i++)
        std::cout << env_vars[i] << std::endl;
    std::cout << CYAN " -- CGI ENV -- end" RESET << std::endl;
    
    std::cout << CYAN " -- CGI ARGV -- start" RESET << std::endl;
    for (size_t i = 0; i < argv_vars.size() - 1; i++)
        std::cout << argv_vars[i] << std::endl;
    std::cout << CYAN " -- CGI ARGV -- end" RESET << std::endl;
}

/**
 * @brief execute CGI script
 */

int    CGI::execute()
{
	int		pipe_fd[4];

    // SET CGI status to ON
    _on = true;
    // open 2 pipes :
    // [0] : cgi script stdin linked to [1] 
    // [2] : cgi script stdout linked to [3]
	if (pipe2(pipe_fd, O_NONBLOCK) == -1 || pipe2(&(pipe_fd[2]), O_NONBLOCK) == -1)
    {
        std::cout << RED "Fatal : failed to open create pipes" RESET << std::endl;
        return (http_tools::get_error_code(errno));
    }
    if ((_pid = fork()) == -1)
    {
        std::cout << RED "Fatal : failed to fork process" RESET << std::endl;
        return (http_tools::get_error_code(errno));
    }
    // Child process
    if (_pid == 0) 
    {
        dup2(pipe_fd[0], 0);
        dup2(pipe_fd[3], 1);
        //fcntl(pipe_fd[2], F_SETFL, O_NONBLOCK);
        close(pipe_fd[1]); // no need, fd[1] will be used by the server to send data to CGI script ([0] aka stdin)
        close(pipe_fd[2]); // no need, fd[2] will be used by the server to receive data from CGI script ([3] aka stdout)
        if (execve(argv_vars[0], &(*argv_vars.begin()), &(*env_vars.begin())) == -1)
        {
            close(pipe_fd[0]);
            close(pipe_fd[3]);
            //return (http_tools::get_error_code(errno));     // does this work ??
        }
        exit(EXIT_SUCCESS);
    }
    // Parent process
    close(pipe_fd[0]);
    close(pipe_fd[3]);
    _fd_in = pipe_fd[1];
    _fd_out = pipe_fd[2];
    return (SUCCESS);
}

/**
 * @brief Push data to CGI script
 * @param content data to be pushed
 */

int CGI::push_data(std::string content)
{
    // Send content of the request'body to CGI script
    int ret = write(_fd_in, content.c_str(), content.size());
    if (ret < 0)
        return (http_tools::get_error_code(errno));
    if ((unsigned int)ret != content.size())
        return (http_tools::get_error_code(errno));
    return SUCCESS;
}

/**
 * @brief send eof to CGI script
 */

int CGI::close_fd_in()
{
    if(close(_fd_in) < 0)
        return (http_tools::get_error_code(errno));
    return SUCCESS;
}

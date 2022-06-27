/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toni <toni@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 00:40:17 by sshakya           #+#    #+#             */
/*   Updated: 2022/04/21 10:00:35 by toni             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

#ifndef CGI_HPP
#define CGI_HPP


/**
 * @brief CGI - Class dedicated to the running of the CGI script
 */
class CGI
{
    public:
        bool    _on;
        pid_t   _pid;        // PID of the script

    protected: 
        int     _fd_in;     // Write FD
        int     _fd_out;    // Read FD
        
        std::vector<std::string> _env;
        std::vector<std::string> _argv;
        
        std::vector<char *> env_vars;
        std::vector<char *> argv_vars;
        
        void    convert(std::vector<std::string> &args, std::vector<char *> &new_vars);

    public:

        // Constructor
        CGI();
        CGI(const CGI &copy);
        
        // Destructor
        ~CGI() {}
       
        void    set_variables(std::vector<std::string> env, std::vector<std::string> argv);
        int     execute(); // execute CGI script (pipe + fork + exec)
        int     push_data(std::string content);     // push data to the CGI script
        int     close_fd_in();                      // send eof to the CGI script

        // Getters
        int     get_fd_in() const;                         
        int     get_fd_out() const;
        std::vector<std::string> get_env() const;
        std::vector<std::string> get_argv() const;
};

#endif  // CGI_HPP
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshakya <sshakya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/16 13:43:31 by sshakya           #+#    #+#             */
/*   Updated: 2022/05/20 20:52:21 by sshakya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class Parsing
{
    public:
        typedef std::vector<std::string>        t_elems;
        typedef std::vector<t_elems>            t_blocks;
        typedef std::vector<t_server_block>    virtual_servers;

    public:
        size_t          _max_body_size;
        
        Parsing(const char * config_file);
        virtual_servers get_virtual_servers();
        
        class InvalidFile : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };
        
        class InvalidConfig : public std::exception
        {
            public:
                virtual const char *what() const throw();
        };

    private:
        t_elems         _elements;
        t_blocks        _tmp_blocks;
        virtual_servers _server_blocks;
        
        void extract_blocks();
        void create_server();
        void extract_locations(int server_id);
        void set_defaults(t_location_block *tmp, std::string location);
        void set_locations(t_location_block *tmp_loc, t_elems::iterator it, int *exit);
        bool extract_max_body_size(std::string first_line);

};

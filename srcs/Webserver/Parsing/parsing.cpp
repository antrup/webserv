/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/05 13:21:38 by atruphem          #+#    #+#             */
/*   Updated: 2022/05/21 14:44:31 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

const char *Parsing::InvalidFile::what() const throw()
{
    return "Config file does not exist\n";
}

const char *Parsing::InvalidConfig::what() const throw()
{
    return "Config file is not valid\n";
}

Parsing::Parsing(const char *conf_file)
{
    std::ifstream config(conf_file);
    if (config.fail())
        throw InvalidFile();
    std::string tmp;

    std::cout << BLUE "init parsing" << RESET << std::endl;
    while (getline(config, tmp, '\n'))
        _elements.push_back(tmp);

    std::cout << BLUE "clear comments" << RESET << std::endl;
    std::vector<std::string>::iterator it;
    for (it = _elements.begin(); it != _elements.end(); it++)
    {
        size_t pos = it->find("//");
        if (pos != std::string::npos)
            it->erase(pos);
    }

    std::cout << BLUE "check syntax - { } ;" << RESET << std::endl;
    for (it = _elements.begin(); it != _elements.end(); it++)
    {
        if (it->size() && *it->rbegin() != '{' && *it->rbegin() != '}' && *it->rbegin() != ';')
            throw InvalidConfig();
    }
}

std::ostream &operator<<(std::ostream &stream, const t_server_block &block)
{
    stream << "server name : " << block.server_name;
    stream << " port : " << block.port;
    stream << " host : " << block.host;
    stream << " is default : " << block.is_default;
    stream << std::endl;
    for (size_t i = 0; i < block.locations.size(); i++)
    {
        stream << " root : " << block.locations[i].root;
        stream << " location : " << block.locations[i].location;
        stream << " flags : ";
        if (block.locations[i].flags & M_GET)
            stream << " GET ";
        if (block.locations[i].flags & M_POST)
            stream << "POST ";
        if (block.locations[i].flags & M_DELETE)
            stream << "DELETE ";
        if (block.locations[i].flags & M_LIST_DIR)
            stream << "LIST_DIR ";
        stream << " index file : " << block.locations[i].index_file;
        stream << std::endl;
    }
    return stream;
}


bool Parsing::extract_max_body_size(std::string first_line)
{
    std::istringstream iss(first_line);
    std::istream_iterator<std::string> start(iss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> element(start, end);
    if (element[0] == "max_body_size")
    {
        int bd_size;
        std::stringstream ss(element[1]);
        ss >> bd_size;
        _max_body_size = bd_size;
        return true;
    }
    else
    {
        if (element[0] != "server")
            throw InvalidConfig();
    }
    return false;
}

void Parsing::extract_blocks()
{
    t_server_block block;

    std::vector<std::string> tmp_block;
    std::vector<std::string>::iterator it;

    int brackets = 0;

    for (it = _elements.begin(); it != _elements.end(); it++)
    {
        if(it == _elements.begin() && _elements.size())
        {
            if (extract_max_body_size(_elements[0]))
                it++;
        }
        tmp_block.push_back(*it);
        if (it->find("{") != std::string::npos)
            brackets++;
        if (it->find("}") != std::string::npos)
            brackets--;
        if (!brackets && tmp_block.size() > 1)
        {
            _tmp_blocks.push_back(tmp_block);
            tmp_block.clear();
        }
    }
}

void Parsing::create_server()
{
    std::cout << BLUE "Extract server blocks" << RESET << std::endl;

    std::vector<t_server_block> config_blocks;

    for (size_t i = 0; i < _tmp_blocks.size(); i++)
    {
        int valid = 1;
        std::string::iterator pos;
        t_elems::iterator it;
        for (it = _tmp_blocks[i].begin(); it != _tmp_blocks[i].end(); it++)
        {
            int count = 0;
            for (std::string::iterator str = it->begin(); str != it->end(); str++)
            {
                if (*str == ';')
                {
                    pos = str;
                    count++;
                }
                if (count > 1)
                {
                    std::cout << " added newline please mofo" << std::endl;
                    valid = 0;
                }
            }
            if (valid && count == 1)
                it->erase(pos);
        }
        if (valid)
            extract_locations(i);
    }
    // LOG AND DEBUG
    //for (virtual_servers::iterator it = _server_blocks.begin(); it != _server_blocks.end(); ++it)
    //{
    //    std::cout << *it << std::endl;
    //}
    return;
}

void Parsing::set_defaults(t_location_block *tmp, std::string location)
{
    memset(tmp, 0, sizeof(t_location_block));
    tmp->location = location;
    tmp->flags |= M_LIST_DIR;
    tmp->flags |= M_POST;
    tmp->flags |= M_GET;
    tmp->flags |= M_DELETE;
    tmp->cgi_map = std::map<std::string, std::string>();
    tmp->accept_upload = false;
}

void Parsing::extract_locations(int server_id)
{
    t_server_block tmp;
    memset(&tmp, 0, sizeof(t_server_block));
    tmp.server_name = "default";
    tmp.body_limit = 64000000;
    tmp.is_default = false;
    tmp.error_pages = std::map<int, std::string>();
    for (t_elems::iterator it = _tmp_blocks[server_id].begin(); it != _tmp_blocks[server_id].end(); it++)
    {
        std::istringstream iss(*it);
        std::istream_iterator<std::string> start(iss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> element(start, end);
        if (element.size() > 1)
        {
            if (element[0] == "listen_port")
                tmp.port = element[1];
            if (element[0] == "listen_host")
                tmp.host = element[1];
            if (element[0] == "server_name")
                tmp.server_name = element[1];
            if (element[0] == "error" && element.size() > 2)
            {
                int err_code;
                std::stringstream ss;  
                ss << element[1];  
                ss >> err_code;  
                tmp.error_pages.insert(std::make_pair(err_code, element[2]));
            }
            if (element[0] == "location")
            {
                t_location_block tmp_loc;
                set_defaults(&tmp_loc, element[1]);
                
                int exit = 0;
                while (!exit)
                {
                    it++;
                    set_locations(&tmp_loc, it, &exit);
                }
                tmp.locations.push_back(tmp_loc);
            }
        }
    }
    _server_blocks.push_back(tmp);
}

void Parsing::set_locations(t_location_block *tmp_loc, t_elems::iterator it, int *exit)
{
    std::istringstream iss_loc(*it);
    std::istream_iterator<std::string> start_loc(iss_loc);
    std::istream_iterator<std::string> end_loc;
    std::vector<std::string> element_loc(start_loc, end_loc);
    if (element_loc.size() > 1)
    {
        if (element_loc.size() > 2)
        {
            if (element_loc[0] == "extension")
                tmp_loc->cgi_map.insert(std::pair<std::string, std::string>("." + element_loc[1], element_loc[2]));
        }
        if (element_loc[0] == "index")
            tmp_loc->index_file = element_loc[1];
        if (element_loc[0] == "root")
            tmp_loc->root = element_loc[1];
        if (element_loc[0] == "list_directory" && element_loc[1] == "false")
            tmp_loc->flags &= ~(M_LIST_DIR);
        if (element_loc[0] == "upload")
        {
            tmp_loc->accept_upload = true;
            tmp_loc->upload_dir = element_loc[1];
        }
        if (element_loc[0] == "method")
        {
            tmp_loc->flags &= ~(M_POST);
            tmp_loc->flags &= ~(M_GET);
            tmp_loc->flags &= ~(M_DELETE);
            std::vector<std::string>::iterator it_temp = element_loc.begin() + 1;
            while (it_temp != element_loc.end())
            {
                if (*it_temp == "POST")
                    tmp_loc->flags |= M_POST;
                if (*it_temp == "GET")
                    tmp_loc->flags |= M_GET;
                if (*it_temp == "DELETE")
                    tmp_loc->flags |= M_DELETE;
                it_temp++;
            }
        }
    }
    if (element_loc.size())
    {
        if (element_loc[0] == "}")
            *exit = 1;
    }
}

Parsing::virtual_servers Parsing::get_virtual_servers()
{
	extract_blocks();
	create_server();
    return _server_blocks;
}
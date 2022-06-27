/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atruphem <atruphem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 19:50:31 by atruphem          #+#    #+#             */
/*   Updated: 2022/05/20 19:50:34 by atruphem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DELETE_request.hpp"
#include "Webserver.hpp"

void DELETE_request::action()
{
    // Find matching location block
    locations::iterator location = this->match_location();

    this->build_path_to_resource(location);

    const char *path_to_file = _ressource.c_str();
    int ret = remove(path_to_file);
    if (ret != 0)
    {
        _return_code = http_tools::get_error_code(errno);
        _flags |= S_COMPLETE;
    }
    else
    {
        _return_code = 200;
        _flags |= S_COMPLETE;
    }
    return ;
}

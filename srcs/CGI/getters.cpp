#include "CGI.hpp"


int     CGI::get_fd_out() const
{
    return (_fd_out);
}

int     CGI::get_fd_in() const
{
    return (_fd_in);
}

std::vector<std::string> CGI::get_env() const
{
    return _env;
}

std::vector<std::string> CGI::get_argv() const
{
    return _argv;
}
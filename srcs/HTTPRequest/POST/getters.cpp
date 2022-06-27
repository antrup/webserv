#include "POST_request.hpp"

size_t POST_request::get_body_missing_size() const
{
    return (_body_missing_size);
}

int POST_request::get_target_fd() const
{
    return (_target_fd);
}
#include "HTTPResponse.hpp"
        
        
/**
 * @brief constructor for response with CGI
 */

HTTPResponse::HTTPResponse(int return_code, std::string reply_content, CGI script, error_pages error_map) :
_script(script),
_body(reply_content),
_error_map(error_map),
_return_code(return_code),
_flags(0)
{
    std::cout << "Creating HTTP Response - VALID" << std::endl;
    if (!_script._on)
        _flags |= S_COMPLETE;
    if (_script._on)
        _flags |= S_HEADER;
}

/**
 * @brief constructor for 'error response'
 */

HTTPResponse::HTTPResponse(int return_code, error_pages error_map) :
_error_map(error_map),
_return_code(return_code),
_flags(0)
{
    std::cout << "Creating HTTP Response - ERROR" << std::endl;
    _body = generate_error_page();
    _flags |= S_COMPLETE;
}

#include "webserv.hpp"
#include "HTTPResponse.hpp"

std::string HTTPResponse::generate_reply()
{
    std::ostringstream oss;
    
    if (_flags & S_CGI && _return_code == SUCCESS)
    {
        if (!(_flags & S_BODY))
        {
            oss << "HTTP/1.1 ";
            std::stringstream ss;
            headers::iterator it;
            // Response start line
            if ((it = _cgi_headers.find("Status"))!= _cgi_headers.end())
                oss << it->second << " OK\r\n";
            else
                oss << "200 OK\r\n";
            // Headers
            if ((it = _cgi_headers.find("Content-Type")) != _cgi_headers.end())
                oss << "Content-Type: " << it->second << "\r\n";
            else
                oss << "Content-Type: text/html\r\n"; // We have to find a way to determine the Content-Type
            // Chunked
            if (_flags & S_CHUNKED)
                oss << "Transfer-Encoding: chunked\r\n";
            else
                oss << "Content-Length: " << _body_lenght << "\r\n";
            oss << "\r\n";
            _flags |= S_BODY;
        }
        oss << _body;
    }
    else
    {
        if (_return_code != SUCCESS)
            _body = generate_error_page();
        oss << "HTTP/1.1 ";
        oss << _return_code << " ";
        oss << get_response_header();
        oss << "\r\n";
        oss << "Cache-Control: no-cache, private\r\n";
        oss << "Content-Type: text/html\r\n"; // We have to find a way to determine the Content-Type
        oss << "Content-Length: " << _body.size() << "\r\n";
        oss << "\r\n";
        oss << _body;
    }
    // LOG AND DEBUG
    std::cout << YELLOW "LOG : \n" RESET << oss.str() << std::endl;

    return oss.str();
}
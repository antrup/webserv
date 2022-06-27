#include "POST_request.hpp"

POST_request::POST_request(std::istringstream &request_data, servers_map &s_map, int socket_fd) : 
HTTPRequest(request_data, s_map, socket_fd)
{
    is_chunked_check();
    if (!_is_chunked)
    {
        std::string content_length;

        content_length = _headers["Content-Length"];
        std::stringstream ss(content_length);
        ss >> _body_missing_size;
    }
    get_body(request_data);
}


void    POST_request::get_body(std::istringstream &data)
{
    if (!_is_chunked)
    {
        char buffer[_body_missing_size + 1];
        memset(buffer, 0, _body_missing_size + 1);
        data.read(buffer, _body_missing_size);

        //if ((_body_missing_size -= data.gcount()) == 0)
        //    _flags |= S_COMPLETE;
        _request_body = buffer;
        return;
    }
    else
    {
        _body_missing_size = 1;
        std::string header;
        std::string garbage;
        int content_len = 0;
        while (!data.eof())
        {
            {
                std::getline(data, header, '\n');

                std::stringstream ss(header);
                content_len = 0;
                ss >> std::hex >> content_len;
            }
            if (content_len == 0)
            {
                std::getline(data, garbage, '\n');
                _body_missing_size = 0;
                //_flags |= S_COMPLETE;
                break;
            }
            else
            {
                char buffer[content_len + 1];
                memset(buffer, 0, content_len + 1);
                data.read(buffer, content_len);
                _request_body.append(buffer);
                std::getline(data, garbage, '\n');
            }
        }
    }
}

void POST_request::is_chunked_check()
{
    HTTPRequest::headers::iterator it;
    it = _headers.find("Transfer-Encoding");

    if (it != _headers.end() && it->second.find("chunked") != std::string::npos) 
    {    
        _is_chunked = true;
        return;
    }
    _is_chunked = false;
}
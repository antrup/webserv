#include "Webserver.hpp"

/**
 * @brief peek to the incomming data
 */

std::string  Webserver::sneak_peek(int socket_id)
{
    char buffer[BUFFER_SIZE];
    memset(&buffer, 0, BUFFER_SIZE);
    int ret = recv(_sockets[socket_id].fd, buffer, BUFFER_SIZE, MSG_PEEK);
    // Check if socket is empty
    if (ret == 0)
    {
        data_socket_error(socket_id);
        return std::string();
    }
    if (ret == -1)
    {
        data_socket_error(socket_id);
        return std::string();
    }
    // buffer data
    std::string str_buffer = buffer;
    return (str_buffer);
}

/**
 * @brief handle peek for new request's data
 */

void    Webserver::initial_peek(int socket_id)
{   
    int type = 0;
    size_t request_size = 0;
    size_t end_of_body_pos = 0;
    size_t headers_end_pos = 0;
    
    // LOG AND DEBUG
    std::cout << YELLOW "Initial Peek" RESET << std::endl;

    // Get socket data
    std::string str_buffer = sneak_peek(socket_id);
    if (str_buffer.empty())
        return;

    // Check header end
    headers_end_pos = str_buffer.find(HEADER_END);
    if (headers_end_pos == std::string::npos)
    {
        // LOG AND DEBUG
        std::cout << YELLOW << "Incomplete headers" << RESET << std::endl;
        return;
    }

    // CHECK METHODS
    std::cout << CYAN "Check Method" RESET << std::endl;
    if (str_buffer.find("POST ") == 0)
    {
        type = M_POST;
        int ret = initial_peek_post(end_of_body_pos, request_size, headers_end_pos, str_buffer);
        if (ret == -1)
        {
            handle_bad_request(socket_id, BAD_REQUEST);
            return;
        }
        if (ret == -2)
        {
            handle_bad_request(socket_id, TOO_LARGE);
            return;
        }
    }
    else if (str_buffer.find("GET ") == 0)
    {
        // set type
        type = M_GET;
        std::cout << GREEN "GET found !" RESET << std::endl;
        request_size = headers_end_pos + 4;
    }
    else if (str_buffer.find("DELETE ") == 0)
    {
        // set type
        type = M_DELETE;
        std::cout << GREEN "DELETE found !" RESET << std::endl;
        request_size = headers_end_pos + 4;
    }
    else
    {    
        handle_bad_request(socket_id, BAD_REQUEST);
        return;
    }
    
    if (request_size)
    {
        if (request_size < BUFFER_SIZE)
        {
            if (str_buffer.size() < request_size)
                return;
            create_request(socket_id, request_size, type);
            return;
        }
    }
    create_request(socket_id, BUFFER_SIZE, type);
}

/**
 * @brief check body content length for POST request
 */

int    Webserver::initial_peek_post(size_t &end_of_body_pos, size_t &request_size, size_t headers_end_pos, std::string &str_buffer)
{
    std::cout << GREEN "POST found !" RESET << std::endl;

    // Check encoding
    std::cout << CYAN "Check chunked" RESET << std::endl;
    if (str_buffer.find("chunked") != std::string::npos)
    {
        std::cout << GREEN "chunks found !" RESET << std::endl;
        request_size = read_chunks(str_buffer, headers_end_pos);
        if (request_size < 0)
            return -2;
        // LOG AND DEBUG
        std::cout << GREY << "Request size:" << request_size << RESET << std::endl;
    }
    else
    {
        // LOG AND DEBUG
        std::cout << CYAN "Check Content-Length" RESET << std::endl;

        size_t content_length_pos = str_buffer.find("Content-Length:");
        if (content_length_pos != std::string::npos)
        {
            // LOG AND DEBUG
            std::cout << GREEN "Content-Length found !" RESET << std::endl;

            size_t content_length = 0;
            std::string content_lenght_str = str_buffer.substr(content_length_pos + 15, str_buffer.find("\r\n", content_length_pos) - 15 - content_length_pos);
            std::stringstream ss(content_lenght_str);
            ss >> content_length;
            if (content_length > _parsing._max_body_size)
                return (-2);
            end_of_body_pos = headers_end_pos + 3 + content_length;
            request_size = end_of_body_pos + 1;
        }
        else
            return (-1);
    }
    return(0);
}

void Webserver::additionnal_peek(int socket_id, POST_request *request)
{
    // LOG AND DEBUG
    std::cout << YELLOW "Additional peek" RESET << std::endl;
    std::string str_buffer = sneak_peek(socket_id);
    size_t data_size = 0;
    
    // Check encoding
    if (request->_is_chunked)
        data_size = read_chunks(str_buffer, 0);
    else
        data_size = request->get_body_missing_size();
    if (data_size)
    {
        if (data_size < BUFFER_SIZE)
        {
            if (str_buffer.size() < data_size)
                return;
            get_additionnal_data(socket_id, data_size, request);
            return;
        }
    }
    get_additionnal_data(socket_id, BUFFER_SIZE, request);
}



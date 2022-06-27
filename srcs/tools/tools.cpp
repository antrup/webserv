#include "webserv.hpp"

namespace http_tools
{

/**
 * @brief return HTTP error code from errno
 * 
 * @param error_no errno
 * @return HTTP error code
 */

int get_error_code(int error_no)
{
    std::cout << RED "ERRNO detected - " << error_no << RESET << std::endl;
    if (error_no == EACCES) /* Access denied */
        return FORBIDDEN;
    else if (error_no == ENFILE) /* too many open files */
        return INTERNAL_ERROR;
    else if (error_no == EISDIR) /* is directory */
        return FORBIDDEN;
    else if (!error_no)
        return NOT_ALLOWED;
    else
        return NOT_FOUND;
}

/**
 * @brief Get the HOST IP
 */
std::string     get_host(struct sockaddr_in *addr_v4)
{
    unsigned char *bytes = reinterpret_cast<unsigned char *>(&addr_v4->sin_addr);

    int ip;
    std::ostringstream oss;

    for (int i = 0; i < 4; i++)
    {
        ip = static_cast<int>(bytes[i]);
        oss << ip;
        if (i != 3)
            oss << ".";
    }
    std::string ret = oss.str();
    return ret;
}

/**
 * @brief Get the PORT
 */
std::string     get_port(struct sockaddr_in *addr_v4)
{
    uint16_t tp_port;
    tp_port = ntohs(addr_v4->sin_port);
    std::ostringstream oss;
    oss << tp_port;
    std::string ret = oss.str();
    return ret; 
}
/**
 * @brief Get the ipv4 addr from FD
 */
struct sockaddr_in get_ipv4_addr(int fd)
{
    struct sockaddr addr;
    socklen_t len = sizeof(addr);
    if (getsockname(fd, &addr, &len))
        perror("cant get sockanme");
    return *(reinterpret_cast<sockaddr_in*>(&addr));
}

/**
 * @brief Get the Client IP
 */
std::string get_client_ip(int fd)
{
    struct sockaddr addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, &addr, &len))
        perror("cant get sockanme");
    struct sockaddr_in addr_v4 = *(reinterpret_cast<sockaddr_in*>(&addr));
    std::string client_ip =  get_host(&addr_v4);
    std::string client_port = get_port(&addr_v4);
    std::string client = client_ip + ":" + client_port;
    std::cout << BLUE << "Client IP : " << RESET;
    std::cout << CYAN << client_ip << RESET;
    std::cout << ":";
    std::cout << PURPLE << client_port << RESET << std::endl;

    return client;
}

/**
 * @brief Generate STD HTML headers
 */

std::string generate_html_header()
{
    std::ostringstream oss;
 
    oss << "<!DOCTYPE html>\n";
    oss << "<html>\n";
    oss << "<head>\n";
    oss << "<meta charset=\"utf-8\"/>\n";
    oss << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
    oss << "<title>Web Server!</title>\n";
    oss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
    oss << "</head>";
    return oss.str();
}

}   //  END NAMESPACE
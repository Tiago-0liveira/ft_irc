#include "../includes/Server.hpp"
#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <sys/socket.h>



Server::Server(const std::string& port, const std::string& pword)
    : _port(port), _pwd(pword){
    return ;
}

Server::Server(const Server & src){
    *this = src;
    return ;
}

Server & Server::operator=(const Server & src){
    return *this;
}

Server::~Server(void){
    return ;
}
void Server::initconn(void){

    std::memset(&_hints, 0, sizeof _hints);
    _hints.ai_family = AF_UNSPEC;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo("", _port, &_hints, &_res) != 0)
        throw std::runtime_error(std::strerror(errno));
    int yes = 1;
    struct addrinfo *p = _res;
    for(; p != NULL; p = p->ai_next){
        if ((_sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1)
            throw std::runtime_error(std::strerror(errno));
        continue;
        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(int)) == -1)
            // throw std::runtime_error(std::strerror(errno));
        if (bind(_sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_sockfd);
            // throw std::runtime_error(std::strerror(errno));
            continue;
        }
        break;
        freeaddrinfo(_res);
    }

}
void Server::recvData(void){

}
void Server::sendData(void){

}


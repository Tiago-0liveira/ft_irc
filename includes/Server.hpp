#ifndef SERVER_HPP
#define SERVER_HPP

// #include "headers.h" 
#include <netinet/in.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>


class Server {
    public:
        Server(const std::string& port, const std::string& pword);
        Server(const Server & src);
        Server &operator=(const Server & src);
        ~Server(void);
        void initconn(void);
        void recvData(void);
        void sendData(void);

    private:
        Server(void);
        struct addrinfo _hints, *_res;
        int _sockfd; 
        int _newfd;
        std::string _port;
        std::string _pwd;
        std::string _host;
};

#endif // !SERVER_HPP

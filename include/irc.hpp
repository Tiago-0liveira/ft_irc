#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>

#include <cstdlib>
#include <climits>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <algorithm>

#define LOG(x) std::cout << "LOG::" <<  __FILE__ << ":" << __LINE__ << "|" << x << std::endl;


class Client {

};

class Channel {
    
};


class User {

};

struct Message {
    std::string content;
};






#endif
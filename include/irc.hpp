#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>

#include <cerrno>
#include <climits>
#include <cstdlib>

#include <algorithm>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define LOG(x) std::cout << "LOG::" << __FILE__ << ":" << __LINE__ << "|" << x << std::endl;

class User
{
};

#endif

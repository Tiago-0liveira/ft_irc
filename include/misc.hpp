#ifndef MISC_HPP
#define MISC_HPP

#include <cerrno>
#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/socket.h>

int         parse_port(char* str, bool& error);
std::string format(const std::string messageFormat, ...);
// TODO: this should be replaced in the future
void sendMessage(int clientSocket, const std::string& message);

#endif
#ifndef MISC_HPP
#define MISC_HPP

#include <cstdlib>
#include <climits>
#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <sys/socket.h>

int parse_port(char *str, bool &error);
std::string format(const std::string messageFormat, ...);
// TODO: this should be replaced in the future
void sendMessage(int clientSocket, const std::string& message);

#endif
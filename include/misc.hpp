#ifndef MISC_HPP
#define MISC_HPP

#include <cerrno>
#include <climits>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <vector>

class Server;
class Client;
#define SERVER_NAME "mariairc"
#define DEFAULT_HOST "127.0.0.1"
#define AVAILABLE_USER_MODES "iswo"
#define AVAILABLE_CHANNEL_MODES "opitlb"
#define BACKLOG 5
#define MAXCLI 5
#define BUFSIZE 128
#define SERVER_VERSION "11.27.24"
#define MAX_CHAN_PER_USR 10
#define MAX_USR_PER_CHAN 10
#define MAX_USR_NICK_LEN 9
#define MAX_CHAN_NAME_LEN 200

int                      parse_port(char* str, bool& error);
std::vector<std::string> strSplit(std::string const& s, char delim);
std::string              format(const std::string messageFormat, ...);
// TODO: this should be replaced in the future
void sendMessage(int clientSocket, const std::string& message);
void msgOfTheDay(Server& srv, Client& cli);
#include <Client.hpp>
#include <Server.hpp>

#endif

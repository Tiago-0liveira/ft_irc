#include "Client.hpp"
#include <misc.hpp>
#include <errors.hpp>

int parse_port(char* str, bool& error)
{
    long n = atol(str);
    if (errno == ERANGE || n > INT_MAX || n < INT_MIN)
    {
        error = true;
        return 0;
    }
    return static_cast<int>(n);
}

std::string format(const std::string messageFormat, ...)
{
    const int bufferSize = 1024;
    char      buffer[bufferSize];

    va_list args;
    va_start(args, messageFormat);

    std::vsnprintf(buffer, bufferSize, messageFormat.c_str(), args);

    va_end(args);

    return std::string(buffer);
}

void sendMessage(int clientSocket, const std::string& message)
{
    send(clientSocket, message.c_str(), message.length(), 0);
}

void msgOfTheDay(Server& srv, Client& cli){

    send_reply(cli, 001, RPL_WELCOME(cli.getNick(), cli.getUser(), cli.getHost()));
	send_reply(cli, 002, RPL_YOURHOST(srv.getName(), SERVER_VERSION));
	send_reply(cli, 003, RPL_CREATED(srv.getDateOfCreation()));
	send_reply(cli, 004, RPL_MYINFO(srv.getName(), SERVER_VERSION, AVAILABLE_USER_MODES, AVAILABLE_CHANNEL_MODES));
	send_reply(cli, 375, RPL_MOTDSTART(srv.getName()));
	send_reply(cli, 372, RPL_MOTD(std::string("Welcome to my IRC server!")));
	send_reply(cli, 376, RPL_ENDOFMOTD());
}

#ifndef COMMANDS
#define COMMANDS

#include "Client.hpp"
#include "Message.hpp"

typedef void (*FuncPtr)(Client& cli, Message& msg);

void passCommand(Client& cli, Message& msg);
void nickCommand(Client& cli, Message& msg);
void userCommand(Client& cli, Message& msg);
void pingCommand(Client& cli, Message& msg);
void pongCommand(Client& cli, Message& msg);
void privmsgCommand(Client& cli, Message& msg);
void noticeCommand(Client& cli, Message& msg);
// void pongCommand(Client& cli, Message& msg);

#endif // !COMMANDS

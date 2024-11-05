#ifndef COMMANDS
#define COMMANDS

#include "Client.hpp"
#include "Message.hpp"


void passCommand(Client& cli, Message& msg);
void nickCommand(Client& cli, Message& msg);
void userCommand(Client& cli, Message& msg);



#endif // !COMMANDS

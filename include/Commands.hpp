#ifndef COMMANDS
#define COMMANDS

#include "Client.hpp"
#include <string>

typedef void (*FuncPtr)(Client& cli, std::string& msg);

void passCommand(Client& cli, std::string& msg);
void nickCommand(Client& cli, std::string& msg);
void userCommand(Client& cli, std::string& msg);
void pingCommand(Client& cli, std::string& msg);
void privmsgCommand(Client& cli, std::string& msg);
void noticeCommand(Client& cli, std::string& msg);
void ignoreCommand(Client& cli, std::string& msg);
void joinCommand(Client& cli, std::string& msg);
void modeCommand(Client& cli, std::string& msg);
void whoCommand(Client& cli, std::string& msg);
void partCommand(Client& cli, std::string& msg);
void topicCommand(Client& cli, std::string& msg);
void inviteCommand(Client& cli, const std::string& msg);
// void pongCommand(Client& cli, std::string& msg);

#endif // !COMMANDS

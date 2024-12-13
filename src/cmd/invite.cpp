#include "errors.hpp"
#include <Client.hpp>
#include <Commands.hpp>
#include <Server.hpp>
#include <sstream>
#include <string>

void inviteCommand(Client& cli, std::string& msg)
{
    Server*            ptr = cli.getServer();
    std::string        cmd, nick, channel;
    std::istringstream ss(msg);

    ss >> cmd;
    ss >> nick;
    ss >> channel;

    if (nick.empty() || channel.empty())
        send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    Client*  invitedCli = ptr->findClient(nick);
    Channel* chan       = ptr->findChannel(channel);
    if (invitedCli == NULL)
        send_error(cli, ERR_NOSUCHNICK, cmd);
    chan->addClient(*invitedCli);
    chan->broadcastMessage(cli, RPL_INVITING(invitedCli->getNick(), chan->getName()));
    return;
}

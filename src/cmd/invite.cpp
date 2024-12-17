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
        return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    Client*  invitedCli = ptr->findClient(nick);
    Channel* chan       = ptr->findChannel(channel);
    if (invitedCli == NULL)
        return send_error(cli, ERR_NOSUCHNICK, cmd);
    chan->inviteClient(cli, *invitedCli);
    return;
}

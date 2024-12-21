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

    if (!cli.isAuth() || !cli.isReg())
    {
        send_error(cli, ERR_NOTREGISTERED, cmd, false);
        return;
    }
    else if (nick.empty() || channel.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    Client*  invitedCli = ptr->findClient(nick);
    Channel* chan       = ptr->findChannel(channel);
    if (invitedCli == NULL)
        return send_error(cli, ERR_NOSUCHNICK, cmd);
    else if (chan == NULL)
        return send_error(cli, ERR_NOSUCHCHANNEL, cmd);
    else if (chan->isMember(cli) == false)
        return send_error(cli, ERR_NOTONCHANNEL, cmd);
    else if (chan->isMember(*invitedCli) == false)
        return send_error(cli, ERR_USERONCHANNEL, cmd);
    else if (chan->getInviteOnly() == true && chan->isOp(cli) == false)
        return send_error(cli, ERR_CHANOPRIVSNEEDED, cmd);
    chan->inviteClient(cli, *invitedCli);
    return;
}

#include "errors.hpp"
#include <Client.hpp>
#include <Commands.hpp>
#include <Server.hpp>
#include <sstream>
#include <string>

void inviteCommand(Client& cli, const std::string& msg)
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
    if (chan == NULL)
        send_error(cli, ERR_NOSUCHCHANNEL, cmd);
    else if (chan->isMember(cli) == false)
        send_error(cli, ERR_NOTONCHANNEL, cmd);
    // TODO: deal with the of channel being invite-only
    //  else if ( ptr->findChannel(channel)->
    //  && ptr->findChannel(channel)->isOp(*ptr->findClient(nick)) == false)
    // send_error(cli, ERR_CHANOPRIVSNEEDED, cmd);
    else if (invitedCli != NULL && chan->isMember(*invitedCli) == true)
        send_error(cli, ERR_USERONCHANNEL, cmd);
    chan->addClient(*invitedCli);
    chan->broadcastMessage(cli, RPL_INVITING(invitedCli->getNick(), chan->getName()));
    return;
}

#include "errors.hpp"
#include <Client.hpp>
#include <Commands.hpp>
#include <Server.hpp>
#include <cstddef>
#include <sstream>
#include <string>

void kickCommand(Client& cli, std::string& msg)
{
    Server*                                  ptr = cli.getServer();
    std::vector<std::string>::const_iterator it;
    std::set<std::string>                    dupChan;
    std::set<std::string>                    dupNick;
    std::string                              cmd, nick, channel, comment;
    std::istringstream                       ss(msg);

    ss >> cmd;
    ss >> channel;
    ss >> nick;

    if (!cli.isAuth() || !cli.isReg())
    {
        send_error(cli, ERR_NOTREGISTERED, cmd, false);
        return;
    }
    std::vector<std::string> nickVector = strSplit(nick, ',');
    std::vector<std::string> chanVector = strSplit(channel, ',');
    if (nickVector.empty() || chanVector.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, "KICK");
    else if (nickVector.size() != chanVector.size())
        return send_error(cli, ERR_NEEDMOREPARAMS, "KICK");
    std::string kickMsg;
    std::size_t colon_pos = msg.find(':');
    if (colon_pos != std::string::npos)
        kickMsg = msg.substr(colon_pos + 1);
    for (size_t i = 0; i < chanVector.size(); ++i)
    {
        if (dupChan.insert(chanVector[i]).second == false ||
            dupNick.insert(nickVector[i]).second == false)
        {
            return send_error(cli, ERR_NEEDMOREPARAMS, "KICK");
        }
        Channel* chan = ptr->findChannel(chanVector[i]);
        if (chan == NULL)
            return send_error(cli, ERR_NOSUCHCHANNEL, "KICK");
        else if (!chan->isOp(cli))
            return send_error(cli, ERR_CHANOPRIVSNEEDED, cli.getNick() + " " + chan->getName(),
                              false);
        Client& kickedClient = *ptr->findClient(nickVector[i]);
        if (kickMsg.empty())
            chan->removeClient(kickedClient, RPL_KICK(kickedClient.getMessageNameBase(),
                                                      chan->getName(), kickedClient.getNick()));
        else
            chan->removeClient(kickedClient,
                               RPL_KICKWMSG(kickedClient.getMessageNameBase(), chan->getName(),
                                            kickedClient.getNick(), kickMsg));
    }
    return;
}

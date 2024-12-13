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
    ss >> comment;

    (void)comment;
    std::vector<std::string> nickVector = strSplit(nick, ',');
    std::vector<std::string> chanVector = strSplit(channel, ',');
    if (nickVector.empty() || chanVector.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, "KICK");
    else if (nickVector.size() != chanVector.size())
        return send_error(cli, ERR_NEEDMOREPARAMS, "KICK");
    for (size_t i = 0; i < chanVector.size(); ++i)
    {
        if (dupChan.insert(chanVector[i]).second == false ||
            dupNick.insert(nickVector[i]).second == false)
        {
            return send_error(cli, ERR_NEEDMOREPARAMS, "KICK");
        }
        else if (ptr->findChannel(chanVector[i]) == NULL)
            return send_error(cli, ERR_NOSUCHCHANNEL, "KICK");
        ptr->findChannel(chanVector[i])->removeClient(*ptr->findClient(nickVector[i]));
    }
    return;
}

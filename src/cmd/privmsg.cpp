#include "Server.hpp"
#include "irc.hpp"
#include <Client.hpp>
#include <Commands.hpp>
#include <cstddef>
#include <errors.hpp>
#include <misc.hpp>
#include <sstream>
#include <string>

void privmsgCommand(Client& cli, std::string& msg)
{
    std::vector<std::string>::const_iterator it;
    std::set<std::string>                    dupControl;
    std::istringstream                       stream(msg);
    std::string                              cmd, targets;
    std::vector<std::string>                 text = strSplit(msg, ':');
    Server*                                  ptr  = cli.getServer();
    stream >> cmd;
    stream >> targets;

    if (!cli.isAuth() || !cli.isReg())
    {
        send_error(cli, ERR_NOTREGISTERED, cmd, false);
        return;
    }
    if (targets.empty())
    {
        send_error(cli, ERR_NORECIPIENT, cmd);
        return;
    }
    else if (text.size() == 1)
    {
        send_error(cli, ERR_NOTEXTTOSEND, cmd);
        return;
    }
    text[1]                             = ":" + text[1];
    std::vector<std::string> newTargets = strSplit(targets, ',');
    for (it = newTargets.begin(); it != newTargets.end(); it++)
    {
        std::ostringstream os;
        os << USER_ID(cli.getNick(), cli.getUser()) << " " << cmd << " " << *it << " " << text[1]
           << "\r\n";
        if (dupControl.insert(*it).second == false)
        {
            send_error(cli, ERR_TOOMANYTARGETS, cmd);
            continue;
        }
        Channel* chan = ptr->findChannel(*it);
        if (chan != NULL)
        {
            if (!chan->isMember(cli))
            {
                send_error(cli, ERR_NOTONCHANNEL, cmd);
                continue;
            }
            if (chan->broadcastMessage(cli, os.str()) == false)
            {
                send_error(cli, ERR_CANNOTSENDTOCHAN, os.str());
                continue;
            }
        }
        else if (ptr->findClient(*it) != NULL)
        {
            ptr->findClient(*it)->setSendBuf(os.str());
        }
        else
            send_error(cli, ERR_NOSUCHNICK, cmd);
    }
}

#include "Server.hpp"
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
    std::string                              cmd, targets, text;
    Server*                                  ptr = cli.getServer();
    stream >> cmd;
    stream >> targets;
    stream >> text;
    if (targets.empty())
    {
        send_error(cli, ERR_NORECIPIENT, cmd);
        return;
    }
    else if (text.empty())
    {
        send_error(cli, ERR_NOTEXTTOSEND, cmd);
        return;
    }
    std::vector<std::string> newTargets = strSplit(targets, ',');
    for (it = newTargets.begin(); it != newTargets.end(); it++)
    {
        LOG(*it);
        if (dupControl.insert(*it).second == false)
            send_error(cli, ERR_TOOMANYTARGETS, cmd);
        if (ptr->findChannel(*it) != NULL)
        { // TODO: The thing below needs changing
            if (ptr->findChannel(*it)->broadcastMessage(cli, text, 0) == false)
                return send_error(cli, ERR_CANNOTSENDTOCHAN, text);
        }
        else if (ptr->findClient(*it) != NULL)
            return sendMessage(ptr->findClient(*it)->getFd(), text);
        else
            return send_error(cli, ERR_NOSUCHNICK, cmd);
    }
}

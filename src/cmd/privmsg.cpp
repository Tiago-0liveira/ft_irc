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
    std::istringstream stream(msg);
    std::string        cmd, targets, text;
    Server*            ptr = cli.getServer();
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
    else if (targets.find(","))
    {
        std::vector<std::string>::const_iterator it;
        std::vector<std::string>                 newTargets = strSplit(msg, ',');
        for (it = newTargets.begin(); it != newTargets.end(); it++)
        {
            if (ptr->findChannel(*it) != NULL)
            { // TODO: The thing below needs changing
                if (ptr->findChannel(*it)->broadcastMessage(cli, text, 0) == false)
                    return send_error(cli, ERR_CANNOTSENDTOCHAN, text);
            }
            else if (ptr->findClient(*it) != NULL && (*it->begin() != '#' || *it->begin() != '&'))
                sendMessage(ptr->findClient(*it)->getFd(), text);
            else
                send_error(cli, ERR_NOSUCHNICK, cmd);
        }
    }
}

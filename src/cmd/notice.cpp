#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/errors.hpp"
#include <sstream>

/*
    Command: NOTICE
    Parameters: <nickname> <text>

    The NOTICE message is used similarly to PRIVMSG.  The difference
    between NOTICE and PRIVMSG is that automatic replies must never be
    sent in response to a NOTICE message.  This rule applies to servers
    too - they must not send any error reply back to the client on
    receipt of a notice.

    The object of this rule is to avoid loops
    between a client automatically sending something in response to
    something it received.

    This is typically used by automatons (clients
    with either an AI or other interactive program controlling their
    actions) which are always seen to be replying lest they end up in a
    loop with another automaton.

    Numeric replies:
        ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
        ERR_NOSUCHNICK
        RPL_AWAY
*/

void noticeCommand(Client& cli, std::string& msg)
{
    std::string cmd, nick, text;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> nick;
    stream >> text;

    if (nick.empty())
    {
        send_error(cli, ERR_NORECIPIENT, cmd);
        return;
    }
    else if (text.empty())
    {
        send_error(cli, ERR_NOTEXTTOSEND, cmd);
        return;
    }
    Server* serverPtr = cli.getServer();
    Client* target    = serverPtr->findClient(nick);
    if (!target)
    {
        send_error(cli, ERR_NOSUCHNICK, cmd);
        return;
    }
    sendMessage(target->getFd(), text); 
}

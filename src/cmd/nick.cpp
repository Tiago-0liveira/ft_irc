#include "../../include/Client.hpp"
#include "../../include/errors.hpp"
#include "irc.hpp"
#include <sstream>

/*
 * Command: NICK
   Parameters: <nickname> [ <hopcount> ]

   NICK message is used to give user a nickname or change the previous
   one.  The <hopcount> parameter is only used by servers to indicate
   how far away a nick is from its home server.  A local connection has
   a hopcount of 0.  If supplied by a client, it must be ignored.

   If a NICK message arrives at a server which already knows about an
   identical nickname for another client, a nickname collision occurs.
   As a result of a nickname collision, all instances of the nickname
   are removed from the server's database, and a KILL command is issued
   to remove the nickname from all other server's database. If the NICK
   message causing the collision was a nickname change, then the
   original (old) nick must be removed as well.

   If the server recieves an identical NICK from a client which is
   directly connected, it may issue an ERR_NICKCOLLISION to the local
   client, drop the NICK command, and not generate any kills.
 Numeric Replies:

           ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
           ERR_NICKNAMEINUSE

   Example:

   NICK Wiz                        ; Introducing new nick "Wiz".

   :WiZ NICK Kilroy                ; WiZ changed his nickname to Kilroy.
*/

void nickCommand(Client& cli, std::string& msg)
{
    Server*            p_serv = cli.getServer();
    std::string        cmd, args;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> args;

    if (args.size() == 0)
    {
        send_error(cli, ERR_NONICKNAMEGIVEN, args, false);
        return;
    }
    else if (!Client::validNick(args))
    {
        send_error(cli, ERR_ERRONEUSNICKNAME, args, false);
        return;
    }
    else if (p_serv->findClient(args))
    {
        send_error(cli, ERR_NICKNAMEINUSE, args);
        return;
    }
    std::string oldNick = cli.getNick();
    cli.setNick(args);
    cli.setAuth();
	p_serv->broadcastMessage(cli, MSG_NICK(oldNick, cli.getNick()), false);
}

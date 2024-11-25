#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Message.hpp"
#include "../../include/errors.hpp"

// Command: PONG
// Parameters: <daemon> [<daemon2>]
//
// PONG message is a reply to ping message.  If parameter <daemon2> is
// given this message must be forwarded to given daemon.  The <daemon>
// parameter is the name of the daemon who has responded to PING message
// and generated this message.
//
// Numeric Replies:
//
//        ERR_NOORIGIN                    ERR_NOSUCHSERVER
//
// Examples:
//
// PONG csd.bu.edu tolsun.oulu.fi  ; PONG message from csd.bu.edu to

void pongCommand(Client& cli, Message& msg)
{
    if (msg._args.empty())
    {
        send_error(cli, ERR_NOORIGIN, msg.getCommand());
        return;
    }
    // TODO: complete this command;
}

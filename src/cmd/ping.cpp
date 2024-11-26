#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/errors.hpp"
#include <sstream>
#include <string>

// Command: PING
// Parameters: <server1> [<server2>]
//
// The PING message is used to test the presence of an active client at
// the other end of the connection.  A PING message is sent at regular
// intervals if no other activity detected coming from a connection.  If
// a connection fails to respond to a PING command within a set amount
// of time, that connection is closed.
//
// Any client which receives a PING message must respond to <server1>
// (server which sent the PING message out) as quickly as possible with
// an appropriate PONG message to indicate it is still there and alive.
// Servers should not respond to PING commands but rely on PINGs from
// the other end of the connection to indicate the connection is alive.
// If the <server2> parameter is specified, the PING message gets
// forwarded there.
//
// Numeric Replies:
//
//        ERR_NOORIGIN                    ERR_NOSUCHSERVER
//
// Examples:
//
// PING tolsun.oulu.fi             ; server sending a PING message to
//                                another server to indicate it is still
//                                alive.
//
// PING WiZ                        ; PING message being sent to nick WiZ

void pingCommand(Client& cli, std::string& msg)
{
    std::istringstream stream(msg);
    std::string cmd, arg;
    
    stream >> cmd;
    stream >> arg;

    if (arg.empty())
    {
        send_error(cli, ERR_NOORIGIN, cmd);
        return;
    }
    // else if ()
    // {
    //     send_error(cli, ERR_NOSUCHSERVER, cmd);
    //     return;
    // }
    // Server*     ptr = cli.getServer();
    // std::string ping =
    //     MSG_PRF + ptr->getHost() + " PONG " + ptr->getHost() + " " + msg._args[0] + MSG_END;
}

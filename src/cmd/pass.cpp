#include "../../include/Client.hpp"
#include "../../include/errors.hpp"
#include <iostream>
#include <sstream>
#include <string>

/*
 * Password message
      Command: PASS
   Parameters: <password>

   The PASS command is used to set a 'connection password'.  The
   password can and must be set before any attempt to register the
   connection is made.  Currently this requires that clients send a PASS
   command before sending the NICK/USER combination and servers *must*
   send a PASS command before any SERVER command.  The password supplied
   must match the one contained in the C/N lines (for servers) or I
   lines (for clients).  It is possible to send multiple PASS commands
   before registering but only the last one sent is used for
   verification and it may not be changed once registered.  Numeric
   Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Example:

           PASS secretpasswordhere
 */

// void send_error(Client& cli, int errnum, std::string& arg)

void passCommand(Client& cli, std::string& msg)
{
    std::string        cmd, args;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> args;

    if (args.size() == 0)
    {
        send_error(cli, ERR_NEEDMOREPARAMS, cmd);
        return;
    }
    else if (cli.isReg())
    {
        send_error(cli, ERR_ALREADYREGISTRED, cmd);
        return;
    }
    cli.setPass(args);
}

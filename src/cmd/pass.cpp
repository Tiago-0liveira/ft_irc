#include <iostream>
#include "../../include/errors.hpp"
#include "../../include/Client.hpp"
#include "../../include/Message.hpp"

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

void passCommand(Client& cli, Message& msg){
    if (msg._args.size() == 0){
        send_error(cli, ERR_NEEDMOREPARAMS, msg.getCommand());
        return ;
    }
    else if (cli.isReg()){
        send_error(cli, ERR_ALREADYREGISTRED, msg.getCommand());
        return ;
    }
    cli.setPass(msg._args[0]);
    std::cout<< "User #"<< cli.getFd() << " password added\n";
}


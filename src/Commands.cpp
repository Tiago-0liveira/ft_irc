#include "../includes/Commands.hpp"
#include "../includes/Message.hpp"
#include "../includes/Client.hpp"
#include "../includes/irc.hpp"

//TODO: creating individual function parsers for: 
//NICK, USER, PASS, JOIN, 
//PRIVMSG, OPER, PING, PONG
//And they should the client, the server and the
//message as parameters which means as soon we
//read the full msg on the socket we should
//create the msg type from it 

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

void passCommand(Client* cli, Server *serv, Message* msg){
    if (cli->isAuth() == false)
        cli->setPass(msg->_args);
    //send_reply();
}

void nickCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find() 

} 

void userCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find()

} 

void joinCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find()

}

void privMsgCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find()

}

void OperCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find()

}

void PingPongCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find()

}

void ModeCommand(Client* cli, Server* serv, Message* msg){
    // serv.getClient()->find()

}


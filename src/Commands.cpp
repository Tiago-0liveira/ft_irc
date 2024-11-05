#include "../include/Commands.hpp"
#include "../include/Message.hpp"
#include "../include/Client.hpp"
#include "../include/irc.hpp"

//TODO: creating individual function parsers for: 
//NICK, USER, PASS, JOIN, 
//PRIVMSG, OPER, PING, PONG
//And they should the client, the server and the
//message as parameters which means as soon we
//read the full msg on the socket we should
//create the msg type from it 

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


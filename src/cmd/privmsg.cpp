#include "../../include/Client.hpp"
#include "../../include/Message.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Channel.hpp"
#include "../../include/errors.hpp"

// Command: PRIVMSG
// Parameters: <receiver>{,<receiver>} <text to be sent>
//
// PRIVMSG is used to send private messages between users.  <receiver>
// is the nickname of the receiver of the message.  <receiver> can also
// be a list of names or channels separated with commas.
//
// The <receiver> parameter may also me a host mask  (#mask)  or  server
// mask  ($mask).   In  both cases the server will only send the PRIVMSG
// to those who have a server or host matching the mask.  The mask  must
// have at  least  1  (one)  "."  in it and no wildcards following the
// last ".".  This requirement exists to prevent people sending messages
// to  "#*"  or "$*",  which  would  broadcast  to  all  users; from
// experience, this is abused more than used responsibly and properly.
// Wildcards are  the  '*' and  '?'   characters.   This  extension  to
// the PRIVMSG command is only available to Operators.
//
// Numeric Replies:
//
//        ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//        ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//        RPL_AWAY
//
// Examples:
//
// :Angel PRIVMSG Wiz :Hello are you receiving this message ?
//                             ; Message from Angel to Wiz.
//
// PRIVMSG Angel :yes I'm receiving it !receiving it !'u>(768u+1n) .br ;
//                             Message to Angel.
//
// PRIVMSG jto@tolsun.oulu.fi :Hello !
//                             ; Message to a client on server
//                             tolsun.oulu.fi with username of "jto".
//
// PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
//                             ; Message to everyone on a server which
//                             has a name matching *.fi.
//
// PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
//                             ; Message to all users who come from a
//                             host which has a name matching *.edu.
void privChannel(std::string const& chanName, Client& cli, Server* ptr){
            if (ptr->findChannel(chanName).isMember(cli)){
                ptr->findChannel(chanName).broadcastMessage("MSG", 0);
                return ;
            }
            else 
                return send_error(cli, ERR_CANNOTSENDTOCHAN, "PRIVMSG");
}

void privmsg(Client& cli, Message& msg){
    std::set<std::string>::iterator it;
    Server* ptr = cli.getServer();
    std::set<std::string>targets;

    if (msg._args.empty())
            return send_error(cli, ERR_NORECIPIENT, msg.getCommand());
    else if (msg._args.size() < 2)
        return send_error(cli, ERR_NOTEXTTOSEND, msg.getCommand());
    if (msg._args[0].find(",") != std::string::npos)
        targets = targetSplit(msg._args[0]);
    for (it = targets.begin(); it != targets.end(); it++){
        if (((*it)[0] == '#' || (*it)[0] == '&') && 
                ptr->m_channelSet.count(*it) != 0){
            privChannel(*it, cli, ptr);
        }
        else
            return send_error(cli, ERR_NOSUCHCHANNEL, "PRIVMSG");
        if (ptr->m_nickSet.count(msg._args[0]) != 0){
            // sendMessage(cli.getFd(), )
        }
            return send_error(cli, ERR_NOSUCHNICK, msg.getCommand());
    }
    //if not send ERR_CANNOTSENDTOCHAN
    std::cout<< "User #"<< cli.getFd() << " sent MSG\n";
}

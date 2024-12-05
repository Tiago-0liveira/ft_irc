#include <Client.hpp>
#include <errors.hpp>
#include <irc.hpp>
#include <sstream>

/*
 *  Command: WHO
   Parameters: [<name> [<o>]]

   The WHO message is used by a client to generate a query which returns
   a list of information which 'matches' the <name> parameter given by
   the client.  In the absence of the <name> parameter, all visible
   (users who aren't invisible (user mode +i) and who don't have a
   common channel with the requesting client) are listed.  The same
   result can be achieved by using a <name> of "0" or any wildcard which
   will end up matching every entry possible.

   The <name> passed to WHO is matched against users' host, server, real
   name and nickname if the channel <name> cannot be found.

   If the "o" parameter is passed only operators are returned according
   to the name mask supplied.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_WHOREPLY                    RPL_ENDOFWHO

   Examples:

   WHO *.fi                        ; List all users who match against
                                   "*.fi".

   WHO jto* o                      ; List all users with a match against
                                   "jto*" if they are an operator.

*/

void whoCommand(Client& cli, std::string& msg)
{
    Server*            serverPtr = cli.getServer();
    std::string        cmd, channel_or_name;
    std::istringstream stream(msg);

	stream >> cmd;
	stream >> channel_or_name;

    if (channel_or_name.size() == 0)
    {
        send_error(cli, ERR_NEEDMOREPARAMS, cmd);
        return;
    }
    if (Channel::validName(channel_or_name))
    {
        Channel* chan = serverPtr->findChannel(channel_or_name);
        if (!chan)
        {
            send_error(cli, ERR_NOSUCHCHANNEL, cmd);
            return;
        }
        std::vector<Client*>::iterator it = chan->getMembers().begin();
		std::string namesList = "";
        while (it != chan->getMembers().end())
        {
            Client* client = *it;

			if (chan->isOp(*client))
				namesList += "@";
            namesList += client->getNick() + " ";
            it++;
        }
		send_reply(cli, 353, RPL_NAMREPLY(cli.getUser(), channel_or_name, namesList));
        send_reply(cli, 366, RPL_ENDOFWHO(channel_or_name));
    }
}

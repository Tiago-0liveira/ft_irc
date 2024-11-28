#include "../../include/Client.hpp"
#include "../../include/errors.hpp"
#include "irc.hpp"
#include <sstream>

/*
 * Command: JOIN
   Parameters: <channel>{,<channel>} [<key>{,<key>}]

   The JOIN command is used by client to start listening a specific
   channel. Whether or not a client is allowed to join a channel is
   checked only by the server the client is connected to; all other
   servers automatically add the user to the channel when it is received
   from other servers.  The conditions which affect this are as follows:

        1.  the user must be invited if the channel is invite-only;

        2.  the user's nick/username/hostname must not match any
            active bans;

        3.  the correct key (password) must be given if it is set.

   These are discussed in more detail under the MODE command (see
   section 4.2.3 for more details).

   Once a user has joined a channel, they receive notice about all
   commands their server receives which affect the channel.  This
   includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.  The
   JOIN command needs to be broadcast to all servers so that each server
   knows where to find the users who are on the channel.  This allows
   optimal delivery of PRIVMSG/NOTICE messages to the channel.

   If a JOIN is successful, the user is then sent the channel's topic
   (using RPL_TOPIC) and the list of users who are on the channel (using
   RPL_NAMREPLY), which must include the user joining.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
           ERR_CHANNELISFULL               ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
           RPL_TOPIC

   Examples:

   JOIN #foobar                    ; join channel #foobar.

   JOIN &foo fubar                 ; join channel &foo using key "fubar".

   JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
                                   and &bar using no key.

   JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
                                   and channel #bar using key "foobar".

   JOIN #foo,#bar                  ; join channels #foo and #bar.

   :WiZ JOIN #Twilight_zone        ; JOIN message from WiZ
*/

void joinCommand(Client& cli, std::string& msg)
{
    Server*            serverPtr = cli.getServer();
    std::string        cmd, channelsList, passwordsList;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> channelsList;
    stream >> passwordsList;

    if (channelsList.size() == 0)
    {
        send_error(cli, ERR_NEEDMOREPARAMS, cmd);
        return;
    }
    std::istringstream channelsStream(channelsList);
    std::istringstream passwordsStream(passwordsList);
    std::string channelName, password;
	do {
		channelsStream >> channelName;
		passwordsStream >> password;
		if (!Channel::validName(channelName))
		{
			send_error(cli, ERR_BADCHANMASK, cmd);
			continue;
		}
		Channel *existingChannel = serverPtr->findChannel(channelName);
		if (!existingChannel)
		{
			if (password.size() != 0)
			{
				serverPtr->addNewChannel(Channel(channelName, password));
				existingChannel = serverPtr->getLastAddedChannel();
				existingChannel->addClient(cli, password);
			}
			else
			{
				serverPtr->addNewChannel(Channel(channelName));
				existingChannel = serverPtr->getLastAddedChannel();
				existingChannel->addClient(cli);
			}
		}
	} while (channelName.size() != 0);
}

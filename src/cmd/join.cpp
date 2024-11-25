#include "../../include/Commands.hpp"
#include "../../include/Client.hpp"
#include "../../include/Message.hpp"
#include "../../include/Channel.hpp"
#include "../../include/errors.hpp"

//#FOO BAR
//    Channels names are strings (beginning with a '&' or '#' character) of
//    length up to 200 characters.  Apart from the the requirement that the
//    first character being either '&' or '#'; the only restriction on a
//    channel name is that it may not contain any spaces (' '), a control G
//    (^G or ASCII 7), or a comma (',' which is used as a list item
//    separator by the protocol).

//ERR_NEEDMOREPARAMS
//ERR_NOSUCHCHANNEL
//ERR_TOOMANYCHANNELS
//ERR_BADCHANNELKEY
void joinCommand(Client& cli, Message& msg)
{
	std::set<std::string> targets;
	std::set<std::string> keys;
	Server* ptr = cli.getServer();

	if(msg._args.size() == 0)
		return send_error(cli, ERR_NEEDMOREPARAMS, "JOIN");
	if (msg._args[0].find(",") != std::string::npos)
		targets = targetSplit(msg._args[0]);
	if (msg._args[0].find(",") != std::string::npos)
		keys = targetSplit(msg._args[1]);

	std::set<std::string>::iterator it;
	for(it = targets.begin(); it != targets.end(); it++){
		if ( it->find_first_not_of("#&0123456789abcdefghijklmnopqrstuvxywz")
			!= std::string::npos){
				send_error(cli, ERR_BADCHANNAME, "JOIN");
			}
		// if (ptr->m_channelSet.count(*it) == 0)
		// Channel tmp(*it);
		// tmp.setKey()
		// ptr->m_channels.emplace_back(Channel(*it));
	}
}

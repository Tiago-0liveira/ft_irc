#include <Client.hpp>
#include <Commands.hpp>
#include <errors.hpp>
#include <misc.hpp>
#include <sstream>
#include <string>
 
/* 
	Command: PART
	Parameters: <channel>{,<channel>}

	 The PART message causes the client sending the message to be removed
   from the list of active users for all given channels listed in the
   parameter string.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_NOTONCHANNEL

   Examples:

   PART #twilight_zone             ; leave channel "#twilight_zone"

   PART #oz-ops,&group5            ; leave both channels "&group5" and
                                   "#oz-ops".
 */


void partCommand(Client& cli, std::string& msg)
{
	Server* server_ptr = cli.getServer();
	if (!server_ptr)
		throw std::runtime_error("SERVER_PTR on Client object cannot be null");
    std::istringstream stream(msg);
    std::string cmd, arg;
    
    stream >> cmd;
	stream >> arg;

	if (!cli.isAuth() || !cli.isReg())
	{
		send_error(cli, ERR_NOTREGISTERED, cmd, false);
		return;
	}
    if (arg.empty())
    {
        send_error(cli, ERR_NEEDMOREPARAMS, cmd);
        return;
    }
	std::string leaveMsg = "";
	std::size_t colon_pos = msg.find(':');
    if (colon_pos != std::string::npos)
    {
		leaveMsg = msg.substr(colon_pos + 1);
        if (leaveMsg.empty())
        {
            send_error(cli, ERR_NEEDMOREPARAMS, cmd);
            return;
        }
	}
	std::vector<std::string> channels = strSplit(arg, ',');
	for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		std::string chan_name = *it;
		if (!Channel::validName(chan_name))
		{
			send_error(cli, ERR_NOSUCHCHANNEL, cmd);
			return;
		}
		Channel *chan = server_ptr->findChannel(chan_name);
		if (!chan)
		{
			send_error(cli, ERR_NOSUCHCHANNEL, cmd);
			return;
		}
		if (leaveMsg.empty())
			chan->removeClient(cli, RPL_PART(cli.getMessageNameBase(), chan->getName()));
		else
			chan->removeClient(cli, RPL_PARTWMSG(cli.getMessageNameBase(), chan->getName(), leaveMsg));
	}
}

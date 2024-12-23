#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/errors.hpp"
#include "misc.hpp"
#include <sstream>
#include <string>

/*
      Command: TOPIC
   Parameters: <channel> [<topic>]

   The TOPIC message is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if the channel modes permit this action.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED

     Examples:

   :Wiz TOPIC #test :New topic     ;User Wiz setting the topic.

   TOPIC #test :another topic      ;set the topic on #test to "another
                                   topic".

   TOPIC #test                     ; check the topic for #test.
 */

void topicCommand(Client& cli, std::string& msg)
{
    Server* server_ptr = cli.getServer();
    if (!server_ptr)
        throw std::runtime_error("SERVER_PTR on Client object cannot be null");
    std::istringstream stream(msg);
    std::string        cmd, chan_name, new_topic;

    stream >> cmd;
    stream >> chan_name;

    if (!cli.isAuth() || !cli.isReg())
    {
        send_error(cli, ERR_NOTREGISTERED, cmd, false);
        return;
    }
    if (chan_name.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    Channel* chan = server_ptr->findChannel(chan_name);
    if (!chan)
        return send_error(cli, ERR_NOSUCHCHANNEL, cmd);
    if (!chan->isMember(cli))
        return send_error(cli, ERR_NOTONCHANNEL, cmd);
    std::size_t colon_pos = msg.find(':');
    if (colon_pos != std::string::npos)
    {
        if (!chan->isOp(cli))
            return send_error(cli, ERR_CHANOPRIVSNEEDED, cli.getNick() + " " + chan->getName(),
                              false);
        new_topic = msg.substr(colon_pos + 1);
        if (new_topic.empty())
            return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
        chan->setTopic(new_topic);

        chan->broadcastMessage(
            cli, ":" + cli.getNick() + " " + RPL_TOPIC(chan->getName(), chan->getTopic()), false);
    }
    else
    { /* No colon */
        std::string topic = chan->getTopic();
        if (topic.empty())
            send_reply(cli, 331, RPL_NOTOPIC(chan->getName()));
        else
            send_reply(cli, 332, RPL_TOPIC(chan->getName(), topic));
        return;
    }
}

#include <Client.hpp>
#include <errors.hpp>
#include <irc.hpp>
#include <sstream>
#include <string>

/*
 * Command: MODE

    The MODE command is a dual-purpose command in IRC.  It allows both
    usernames and channels to have their mode changed.  The rationale for
    this choice is that one day nicknames will be obsolete and the
    equivalent property will be the channel.

    When parsing MODE messages, it is recommended that the entire message
    be parsed first and then the changes which resulted then passed on.

    Channel modes:
        Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
               [<ban mask>]

        The MODE command is provided so that channel operators may change the
        characteristics of `their' channel.  It is also required that servers
        be able to change channel modes so that channel operators may be
        created.

        The various modes available for channels are as follows:

                o - give/take channel operator privileges;
                i - invite-only channel flag;
                t - topic settable by channel operator only flag;
                l - set the user limit to channel;
                k - set a channel key (password).

        When using the 'o' and 'b' options, a restriction on a total of three
        per mode command has been imposed.  That is, any combination of 'o'
        and

        Examples:

                Use of Channel Modes:

        MODE #Finnish +im               ; Makes #Finnish channel moderated and
                                        'invite-only'.

        MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on
                                        channel #Finnish.

        MODE #Finnish +v Wiz            ; Allow WiZ to speak on #Finnish.

        MODE #Fins -s                   ; Removes 'secret' flag from channel
                                        #Fins.

        MODE #42 +k oulu                ; Set the channel key to "oulu".

        MODE #eu-opers +l 10            ; Set the limit for the number of users
                                        on channel to 10.

        MODE &oulu +b                   ; list ban masks set for channel.

        MODE &oulu +b *!*@*             ; prevent all users from joining.

        MODE &oulu +b *!*@*.edu         ; prevent any user from a hostname
                                        matching *.edu from joining.

*/

// o - give/take channel operator privileges;
// i - invite-only channel flag;
// t - topic settable by channel operator only flag;
// l - set the user limit to channel;
// k - set a channel key (password).

void modeCommand(Client& cli, std::string& msg)
{
    Server*            serverPtr = cli.getServer();
    std::string        cmd, channel, mode, params;
    std::istringstream stream(msg);

    stream >> cmd;
    stream >> channel;
    stream >> mode;
    stream >> params;

    if (!cli.isAuth() || !cli.isReg())
    {
        send_error(cli, ERR_NOTREGISTERED, cmd, false);
        return;
    }
    if (channel.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    Channel* chan = serverPtr->findChannel(channel);
    if (chan == NULL)
        return send_error(cli, ERR_NOSUCHCHANNEL, cmd);
    if (!chan->isMember(cli))
        return send_error(cli, ERR_NOTONCHANNEL, cmd);
    if (mode.empty())
        return cli.setSendBuf(":" + std::string(SERVER_NAME) + " 324 " + cli.getNick() + " " +
                              channel + " " + chan->getActiveModesInString() + "\r\n");
    if (!(mode[0] == '+' || mode[0] == '-') &&
        mode.find_last_not_of("kitol", 1) != std::string::npos)
        return send_error(cli, ERR_UNKNOWNMODE, cmd);
    else if (mode.size() < 2)
        return send_error(cli, ERR_UNKNOWNMODE, cmd);
    else if (mode[1] == 'o' && serverPtr->findClient(params) == NULL)
        return send_error(cli, ERR_NOSUCHNICK, cmd);
    else if (mode == "+k" && params.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    if (!chan->isOp(cli))
        return send_error(cli, ERR_CHANOPRIVSNEEDED, cmd);
    chan->addMode(cli, mode, params);
    return;
}

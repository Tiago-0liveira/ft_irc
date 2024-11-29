#include <Client.hpp>
#include <Commands.hpp>
#include <errors.hpp>
#include <misc.hpp>
#include <sstream>
#include <string>

void userCommand(Client& cli, std::string& msg)
{
    std::string        cmd, user, mode, unused, realname;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> user;
    stream >> mode;
    stream >> unused;
    stream >> realname;

    if (msg.size() < 4)
    {
        send_error(cli, ERR_NEEDMOREPARAMS, cmd);
        return;
    }
    else if (cli.isAuth() && cli.isPasswordSet() && cli.isReg())
    {
        send_error(cli, ERR_ALREADYREGISTRED, cmd);
        return;
    }
    if (!cli.isAuth() || !cli.isPasswordSet())
        return send_error(cli, ERR_NOTREGISTERED, cmd);
    cli.setUser(user);
    cli.setMode(mode);
    (void)unused;
    cli.setRealname(realname.substr(1));
    cli.setReg();
    // TODO: MesssageOfTheDay func()
    //
    sendMessage(cli.getFd(), RPL_USER(LOCALHOST, cli.getNick().c_str()));
    msgOfTheDay(*(cli.getServer()), cli);
    return;
}

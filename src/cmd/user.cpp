#include <Client.hpp>
#include <Commands.hpp>
#include <errors.hpp>
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
    else if (cli.isAuth())
    {
        send_error(cli, ERR_ALREADYREGISTRED, cmd);
        return;
    }
    if (!cli.isAuth() && !cli.isPasswordSet())
        return;
    cli.setUser(user);
    cli.setMode(mode);
    (void)unused;
    cli.setRealname(realname);
    cli.setReg();
    // TODO: MesssageOfTheDay func()
    return;
}

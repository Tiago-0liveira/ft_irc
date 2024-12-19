#include <Client.hpp>
#include <Commands.hpp>
#include <errors.hpp>
#include <misc.hpp>
#include <sstream>
#include <string>

void userCommand(Client& cli, std::string& msg)
{
    Server*            serv_ptr = cli.getServer();
	if (!serv_ptr)
		throw std::runtime_error("serv_ptr cannot be NULL in client");
    std::string        cmd, user, mode, unused, realname;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> user;
    stream >> mode;
    stream >> unused;
    (void)unused;
    stream >> realname;

    if (user.empty() || mode.empty() || unused.empty() || realname.empty())
        return send_error(cli, ERR_NEEDMOREPARAMS, cmd);
    else if (cli.isAuth() && cli.isPasswordSet() && cli.isReg())
        return send_error(cli, ERR_ALREADYREGISTRED, cmd);

    if (!cli.isAuth() || !cli.isPasswordSet())
        return send_error(cli, ERR_NOTREGISTERED, cmd, false);
    if (serv_ptr->getPassword() != cli.getPass())
		return send_error(cli, ERR_PASSWDMISMATCH, cmd);

	cli.setUser(user);
    cli.setMode(mode);
    cli.setRealname(realname.substr(1));
    cli.setReg();
    cli.setSendBuf(RPL_USER(LOCALHOST, cli.getNick().c_str()));
    msgOfTheDay(*(cli.getServer()), cli);
    return;
}

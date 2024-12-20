#include <Client.hpp>
#include <Commands.hpp>
#include <errors.hpp>
#include <misc.hpp>
#include <sstream>
#include <string>

void quitCommand(Client& cli, std::string& msg)
{
    Server* serv_ptr = cli.getServer();
    if (!serv_ptr)
        throw std::runtime_error("Server cannot be null");
    std::string        cmd;
    std::istringstream stream(msg);
    stream >> cmd;

    std::string leaveMsg  = cli.getNick();
    std::size_t colon_pos = msg.find(':');
    if (colon_pos != std::string::npos)
        leaveMsg = msg.substr(colon_pos + 1);
    cli.setLeaveMsg(leaveMsg);
    serv_ptr->queueDeleteFd(cli.getFd());
    return;
}

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
    std::string        cmd, msg_end;
    std::istringstream stream(msg);
    stream >> cmd;
    stream >> msg_end;

    std::string leaveMsg = cli.getNick();
    std::size_t colon_pos = msg_end.find(':');
    if (colon_pos != std::string::npos)
        leaveMsg = msg_end.substr(colon_pos + 1);
    serv_ptr->queueDeleteFd(cli.getFd());
    return;
}

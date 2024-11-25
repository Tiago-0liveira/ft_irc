#include "../include/errors.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>

std::string errmap(int errcode)
{
    static std::map<int, std::string> m;

    m[ERR_NOSUCHNICK]       = " :No such nick";
    m[ERR_NOSUCHSERVER]     = " :No such server";
    m[ERR_NOSUCHCHANNEL]    = " :No such channel";
    m[ERR_CANNOTSENDTOCHAN] = " :Cannot send to channel";
    m[ERR_TOOMANYCHANNELS]  = " :You have joined too many channels";
    m[ERR_TOOMANYTARGETS]   = " :Duplicate recipients. No message delivered";
    m[ERR_NOORIGIN]         = " :No origin specified";
    m[ERR_NORECIPIENT]      = " :No recipient given";
    m[ERR_NOTEXTTOSEND]     = " :No text to send";
    m[ERR_NOMOTD]           = " :MOTD File is missing";
    m[ERR_NONICKNAMEGIVEN]  = " :No nickname given";
    m[ERR_ERRONEUSNICKNAME] = " :Erroneous nickname";
    m[ERR_NICKNAMEINUSE]    = " :Nickname is already in use.";
    m[ERR_NOTONCHANNEL]     = " :You're not on that channel";
    m[ERR_USERONCHANNEL]    = " :is already on channel";
    m[ERR_NOTREGISTERED]    = " :You have not registered";
    m[ERR_NEEDMOREPARAMS]   = " :Not enough parameters";
    m[ERR_ALREADYREGISTRED] = " :You may not reregister";
    m[ERR_KEYSET]           = " :Channel key already set";
    m[ERR_CHANNELISFULL]    = " :Cannot join channel (+l)";
    m[ERR_UNKNOWNMODE]      = " :is unknown mode char to me";
    m[ERR_INVITEONLYCHAN]   = " :Cannot join channel (+i)";
    m[ERR_BANNEDFROMCHAN]   = " :Cannot join channel (+b)";
    m[ERR_BADCHANNELKEY]    = " :Cannot join channel (+k)";
    m[ERR_BADCHANMASK]      = " :The given channel mask was invalid";
    m[ERR_CHANOPRIVSNEEDED] = " :You're not channel operator";
    m[ERR_UMODEUNKNOWNFLAG] = " :Unknown MODE flag";
    m[ERR_USERSDONTMATCH]   = " :Can't change mode for other users not being IRC operator";
    m[ERR_NOOPERHOST]       = " :No O-lines for your host";
    m[ERR_PASSWDMISMATCH]   = " :Password incorrect";
    m[ERR_BADCHANNAME]      = ":Cannot join channel";

    return m[errcode];
}

void send_error(Client& cli, int errnum, const std::string& arg)
{
    std::ostringstream os;

    os << ":mariairc " << errnum << " * " << arg << errmap(errnum) << "\r\n";

    std::string s = os.str();
    if (send(cli.getFd(), s.c_str(), s.size(), 0) == -1)
        throw std::runtime_error("failure to send");
}

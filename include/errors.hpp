#ifndef ERRORS
#define ERRORS

#pragma once
#include <Client.hpp>
#include <string>

std::string errmap(int errcode);
void        send_error(Client& cli, int errnum, const std::string& arg, bool add_star = true);
void        send_reply(Client& cli, int rpl_code, std::string msg);
void        broadcastNotice(Client& src, Channel& dst, std::string notice);

#define LOCALHOST "localhost"

// MESSAGES
#define MSG_NICK(oldNick, new_nick) (":" + oldNick + " NICK :" + new_nick + "\r\n")

// REPLIES
#define RPL_WELCOME(nick, user, host) (nick + " :Welcome to the Internet Relay Network\r\n")
#define RPL_YOURHOST(servername, version)                                                          \
    (":Your host is " + servername + ", running version " + version + "\r\n")
#define RPL_CREATED(date) (":This server was created " + date + "\r\n")
#define RPL_MYINFO(servername, version, available_user_modes, available_channel_modes)             \
    (":" + servername + " " + version + " " + available_user_modes + " " +                         \
     available_channel_modes + "\r\n")
#define RPL_USER(server, nick) (format(":%s NICK %s\r\n", server, nick))
#define RPL_JOIN(userHost, channel) (userHost + "JOIN " + channel + "\r\n")
#define RPL_PART(userHost, channel) (userHost + "PART " + channel + "\r\n")
#define RPL_PARTWMSG(userHost, channel, msg) (userHost + "PART " + channel + " :" + msg + "\r\n")
#define RPL_QUIT(userHost, channel) (userHost + "QUIT " + channel + "\r\n")
#define RPL_QUITWMSG(userHost, channel, msg) (userHost + "QUIT " + channel + " :" + msg + "\r\n")
#define RPL_KICK(userHost, channel, nick) (userHost + "KICK " + channel + " " + nick + "\r\n")
#define RPL_KICKWMSG(userHost, channel, nick, msg)                                                 \
    (userHost + "KICK " + channel + " " + nick + " :" + msg + "\r\n")
#define RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
#define RPL_MOTD(txt) (txt + "\r\n")
#define RPL_ENDOFMOTD() (":End of /MOTD command\r\n")
#define RPL_WHOREPLY(curr_chan, username, hostname, servname, nick, chanoper, realname)            \
    (curr_chan + " " + username + " " + hostname + " " + servname + " " + nick + " H" + chanoper + \
     " :0 " + realname + "\r\n")
#define RPL_ENDOFWHO(name) (name + " :End of /WHO list\r\n")
#define RPL_CHANNELMODEIS(client, channel, mode) (client + " " + channel + " " + mode + "\r\n")
#define RPL_CHANNELMODEISARGS(client, channel, mode, args)                                         \
    (client + " " + channel + " " + mode + " " + args + "\r\n")
#define RPL_NOTOPIC(channel) ("TOPIC " + channel + " :No topic is set" + "\r\n")
#define RPL_TOPIC(channel, topic) ("TOPIC " + channel + " :" + topic + "\r\n")
#define RPL_NAMREPLY(name, channel, list) (name + " = " + channel + " :" + list + "\r\n")
#define RPL_ENDOFNAMES(channel) (channel + " :End of /NAMES list.\r\n")
#define RPL_CREATIONTIME(channel, creation_time) (channel + " :" + creation_time + "\r\n")
#define RPL_INVITING(client, guest, channel) (client + " " + guest + " " + channel + "\r\n")
#define RPL_INVITE(invited, channel) (" INVITE " + invited + " " + channel + "\r\n")
#define ERR_SERVERISFULL(host)                                                                     \
    ("ERROR :Closing link: (unknown@" + host +                                                     \
     ") [No more connections allowed from your host via this connect class (local)]\r\n")
#define RPL_PONG(user_id, server) (user_id + " PONG " + server + "\r\n")

// NOTICES
#define NTC_MODE(channel, nick, mode) ("MODE " + channel + " " + mode + " " + nick + "\r\n")
#define NTC_LASTOP(nick, channel) ("NOTICE " + nick + " :You are the only operator in " + channel + ". You cannot remove your own operator status.\r\n")

// ERRORS
#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHSERVER 402
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_TOOMANYCHANNELS 405
#define ERR_WASNOSUCHNICK 406 // "<nickname> :There was no such nickname"
#define ERR_TOOMANYTARGETS 407
#define ERR_NOORIGIN 409
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_NOTOPLEVEL 413     // "<mask> :No toplevel domain specified"
#define ERR_WILDTOPLEVEL 414   // "<mask> :Wildcard in toplevel domain"
#define ERR_UNKNOWNCOMMAND 421 // "<command> :Unknown command"
#define ERR_NOMOTD 422
#define ERR_NOADMININFO 423 // "<server> :No administrative info available"
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTRED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_KEYSET 467
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_BADCHANMASK 476
#define ERR_BADCHANNAME 479
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_NOOPERHOST 491
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502
#define ERR_INVALIDMODEPARAM 696

#endif // !ERRORS

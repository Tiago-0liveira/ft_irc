#ifndef REPLIES
#define REPLIES

#include "Client.hpp"

void rpl_welcome(Client& cli);
void rpl_yourhost(Client& cli);
void rpl_created(Client& cli);
void rpl_myinfo(Client& cli);
void rpl_motdstart(Client& cli);
void rpl_motd(Client& cli);
void rpl_endofmotd(Client& cli);
void rpl_umodeis(Client& cli);
void rpl_whoreply(Client& cli);
void rpl_endofwho(Client& cli);
void rpl_chaennelmodeis(Client& cli);
void rpl_notopic(Client& cli);
void rpl_topic(Client& cli);
void rpl_topicwhotime(Client& cli);
void rpl_namereply(Client& cli);
void rpl_endofnames(Client& cli);
void rpl_away(Client& cli);
void rpl_creationtime(Client& cli);
void rpl_banlist(Client& cli);
void rpl_endofbanlist(Client& cli);
void rpl_inviting(Client& cli);
void rpl_serverisfull(Client& cli);

#define RPL_WELCOME(nick, user, host) (":Welcome to the Internet Relay Network\r\n")
#define RPL_YOURHOST(servername, version)                                                          \
    (":Your host is " + servername + ", running version " + version + "\r\n")
#define RPL_CREATED(date) (":This server was created " + date + "\r\n")
#define RPL_MYINFO(servername, version, available_user_modes, available_channel_modes)             \
    (":" + servername + " " + version + " " + available_user_modes + " " +                         \
     available_channel_modes + "\r\n")
#define RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
#define RPL_MOTD(txt) (txt + "\r\n")
#define RPL_ENDOFMOTD() (":End of /MOTD command\r\n")
#define RPL_UMODEIS(user_mode) (user_mode + "\r\n")
#define RPL_WHOREPLY(curr_chan, username, hostname, servname, nick, ircoper, chanoper, realname)   \
    (curr_chan + " " + username + " " + hostname + " " + servname + " " + nick + " H" + ircoper +  \
     chanoper + " :0 " + realname + "\r\n")
#define RPL_ENDOFWHO(name) (name + " :End of /WHO list\r\n")
#define RPL_CHANNELMODEIS(channel, mode) (channel + " :+" + mode + "\r\n")
#define RPL_NOTOPIC(channel) (channel + " :No topic is set" + "\r\n")
#define RPL_TOPIC(channel, topic) (channel + " :" + topic + "\r\n")
#define RPL_TOPICWHOTIME(channel, who, when) (channel + " " + who + " :" + when + "\r\n")
#define RPL_NAMREPLY(channel, list) ("= " + channel + " :" + list + "\r\n")
#define RPL_ENDOFNAMES(channel) (channel + " :End of /NAMES list.\r\n")
#define RPL_AWAY(nick, msg) (nick + " :" + msg + "\r\n")
#define RPL_CREATIONTIME(channel, creation_time) (channel + " :" + creation_time + "\r\n")
#define RPL_BANLIST(channel, mask) (channel + " :" + mask + "\r\n")
#define RPL_ENDOFBANLIST(channel) (channel + " :End of channel ban list\r\n")
#define RPL_INVITING(guest, channel) (guest + " :" + channel + "\r\n")
#define ERR_SERVERISFULL(host)                                                                     \
    ("ERROR :Closing link: (unknown@" + host +                                                     \
     ") [No more connections allowed from your host via this connect class (local)]\r\n")

#endif // !REPLIES

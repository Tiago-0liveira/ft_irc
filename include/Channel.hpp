#ifndef CHANNEL_H
#define CHANNEL_H

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <map>
#include <misc.hpp>
#include <vector>

#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"

class Client;
class Server;

class Channel
{
	static const std::string DEFAULT_PASS;

  private:
    // TODO: change to class Client (for now we're using int)
    // DONE(adilson):added Client pointers as members of channels
    typedef void (Channel::*t_exe)(Client&, std::string, std::string);

    std::string           _channel;
    std::string           _pass;
    std::string           _topic;
    unsigned int          _limit;
    bool                  _is_key;
    bool                  _invite_only;
    bool                  _topic_change;
    bool                  _is_limited;
    std::map<char, int>   _nbr_modes;
    std::map<char, t_exe> _modes;
    std::vector<Client>   _member;
    std::vector<Client>   _op;

  public:
    // static std::string JOIN_MESSAGE;
    // static std::string LEAVE_MESSAGE;
    // static std::string TOPIC_MESSAGE;

	Channel(std::string name, std::string key = DEFAULT_PASS);
    ~Channel();

    // Member functions
    void addClient(Client& client, std::string password = DEFAULT_PASS);
    bool isMember(Client& client);
    void inviteClient(Client& member, Client& invited);
    bool kickClient(std::string clientNick);
    void topic(std::string topic, Client& client);
    bool broadcastMessage(Client& cli, const std::string& message, int exceptFd);

    void addMode(Client& client, std::string mode, std::string argument);
    void inviteMode(Client& client, std::string mode, std::string argument);
    void topicMode(Client& client, std::string mode, std::string argument);
    void keyMode(Client& client, std::string mode, std::string argument);
    bool isOp(Client& client);
    void addOp(Client& client);
    void removeOp(Client& client);
    void operatorMode(Client& client, std::string mode, std::string argument);
    void limitMode(Client& client, std::string mode, std::string argument);
	static bool validName(const std::string &name);

    // Acessers
    std::vector<Client>& getMembers();
    std::vector<Client>& getOp();
    std::map<char, int>& getNbrMode();
    std::string          getName();
    std::string          getpass();
    std::string          getTopic() const;
    void                 setTopic(std::string topic);
	bool				 getInviteOnly() const;
};

#endif

#ifndef CHANNEL_H
#define CHANNEL_H

#include "Client.hpp"
#include "Server.hpp"
#include <errors.hpp>
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
	static const std::string DEFAULT_TOPIC;

private:
	typedef void (Channel::*t_exe)(Client&, std::string, std::string);

	std::string				_channel;
	std::string				_pass;
	std::string				_topic;
	unsigned int			_limit;
	bool					_is_key;
	bool					_invite_only;
	bool					_topic_change;
	bool					_is_limited;
	std::map<char, int>		_nbr_modes;
	std::map<char, t_exe>	_modes;
	std::vector<Client*>	_member; /* Vector of Client* that live inside the server class */
	std::vector<Client*>	_op; /* Vector of Client* ops that live inside the server class */
	Server*					_serv_ptr;

public:
	Channel(std::string name, Server* server, std::string key = DEFAULT_PASS);
	~Channel();

	// Member functions
	std::string						memberList(void);
	void							joinClient(Client& client, std::string key = DEFAULT_PASS);
	void							removeClient(Client &client);
	bool							isMember(Client& client);
	bool							isOp(Client& client);
	void							inviteClient(Client& member, Client& invited);
	std::vector<Client*>::iterator	findIt(std::vector<Client*>::iterator it, std::vector<Client*>::iterator itend, std::string nick);
	void							kickClient(Client &chop, Client &member, std::string reason);
	void							topic(Client& member, std::string topic);
	bool							broadcastMessage(Client& cli, const std::string& message);
	bool							broadcastReply(const std::string& message, int rpl_code);

	void		addMode(Client& client, std::string mode, std::string argument);
	void		inviteMode(Client& client, std::string mode, std::string argument);
	void		topicMode(Client& client, std::string mode, std::string argument);
	void		keyMode(Client& client, std::string mode, std::string argument);
	void		addOp(Client *client);
	void		removeOp(Client *client);
	void		operatorMode(Client& client, std::string mode, std::string argument);
	void		limitMode(Client& client, std::string mode, std::string argument);
	static bool	validName(const std::string& name);

	// Acessers
	std::vector<Client*>&	getMembers();
	std::vector<Client*>&	getOp();
	std::map<char, int>&	getNbrMode();
	std::string				getName();
	std::string				getpass();
	std::string				getTopic() const;
	std::string				getMessageBaseName() const;
	void					setTopic(std::string topic);
	bool					getInviteOnly() const;
};

#endif
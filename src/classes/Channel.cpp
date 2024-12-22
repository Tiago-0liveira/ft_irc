#include <Channel.hpp>
#include <Commands.hpp>
#include <iomanip>
#include <sstream>

const std::string Channel::DEFAULT_PASS = "123";
const std::string Channel::DEFAULT_TOPIC = "No Topic";

Channel::Channel(std::string name, Server* server, std::string key)
	: _channel(name), _pass(key), _topic(DEFAULT_TOPIC), _limit(10), _op(), _serv_ptr(server)
{
	if (_channel[0] == '#' || key != DEFAULT_PASS)
	{
		_is_key = true;
		_pass   = key;
	}
	else
		_is_key = false;

	_topic_change = true;
	_is_limited   = false;
	_invite_only  = false;

	_modes['i'] = &Channel::inviteMode;
	_modes['t'] = &Channel::topicMode;
	_modes['k'] = &Channel::keyMode;
	_modes['o'] = &Channel::operatorMode;
	_modes['l'] = &Channel::limitMode;
}

Channel::~Channel() {}

std::string Channel::memberList(void)
{
	std::vector<Client*>::iterator	mbr;
	std::string			names_list = "";

	for (mbr = _member.begin(); mbr != _member.end(); ++mbr)
	{
		names_list = names_list + (*mbr)->getNick() + " ";
	}
	return (names_list);
}

void Channel::joinClient(Client& client, std::string key)
{
	std::vector<Client*>::iterator	find = std::find(_member.begin(), _member.end(), &client);
	std::string						list = "";

	if (find == _member.end())
	{
		if (key != _pass)
		{
			send_error(client, ERR_BADCHANNELKEY, "JOIN"); //Cannot join channel (+k)
			return;
		}
		if (_invite_only == true)
		{
			std::vector<std::string>::iterator find_invite = std::find(
				client.getChannalInvites().begin(), client.getChannalInvites().end(), _channel);
			if (find_invite == client.getChannalInvites().end())
			{
				send_error(client, ERR_INVITEONLYCHAN, "JOIN"); //Cannot join channel (+i)
				return;
			}
		}
		if (_member.size() == _limit)
		{
			send_error(client, ERR_CHANNELISFULL, "JOIN"); //Cannot join channel (+l)
			return;
		}
		list = memberList();
		_member.push_back(&client);
		client.setSendBuf(RPL_JOIN(client.getMessageNameBase(), _channel));
		std::string topicCommandMsg = "TOPIC " + getName() + "\r\n";
		topicCommand(client, topicCommandMsg);
		send_reply(client, 353, RPL_NAMREPLY(client.getNick(), _channel, list));
		broadcastMessage(client, RPL_JOIN(client.getMessageNameBase(), _channel));
	}
	else
		send_error(client, ERR_USERONCHANNEL, "JOIN"); //is already on channel
	if (_op.size() == 0)
		_op.push_back(&client);
}

void Channel::removeClient(Client& client)
{
	std::vector<Client*>::iterator find = std::find(_member.begin(), _member.end(), &client);
	if (find != _member.end())
	{
		client.setSendBuf(RPL_PART(client.getMessageNameBase(), getName()));
		broadcastMessage(client, RPL_PART(client.getMessageNameBase(), getName()));
		std::vector<Client*>::iterator find_op = std::find(_op.begin(), _op.end(), &client);
		if (find_op != _op.end())
		{
			_op.erase(find_op);
			_member.erase(find);
			if (_op.size() == 0)
			{
				if (_member.size() != 0)
				{
					Client* cli = _member.at(0);
					if (!cli)
						throw std::runtime_error("cli is null, bad error!");
					addOp(cli);
					broadcastMessage(client, getMessageBaseName() +
												NTC_MODE(_channel, cli->getNick(), "+o"));
				}
			}
		}
		else
			_member.erase(find);
	}
}

bool Channel::isMember(Client& client)
{
	std::vector<Client*>::iterator find;

	find = std::find(_member.begin(), _member.end(), &client);
	if (find != _member.end())
		return true;
	return false;
}

bool Channel::isOp(Client& client)
{
	std::vector<Client*>::iterator find;

	find = std::find(_op.begin(), _op.end(), &client);
	if (find != _op.end())
		return true;
	return false;
}

void Channel::inviteClient(Client& member, Client& invited)
{
	if (!isMember(member))
	{
		send_error(member, ERR_NOTONCHANNEL, "INVITE"); //You're not on that channel
		return;
	}
	if (!isOp(member) && _invite_only == true)
	{
		send_error(member, ERR_CHANOPRIVSNEEDED, "INVITE"); //You're not channel operator
		return;
	}
	if (isMember(invited))
	{
		send_error(member, ERR_USERONCHANNEL, "INVITE"); //is already on channel
		return;
	}
	
	std::vector<std::string>::iterator find = std::find(invited.getChannalInvites().begin(), invited.getChannalInvites().end(), _channel);
	if (find == invited.getChannalInvites().end())
	{
		invited.getChannalInvites().push_back(_channel);
		send_reply(invited, 341, RPL_INVITING(invited.getNick(), _channel)); //não sei se isto é a função certa
		//send a message to server that invite was sucessful -> 'invited' "You were invited to join <_channel>"
	}
}

std::vector<Client*>::iterator Channel::findIt(std::vector<Client*>::iterator it, std::vector<Client*>::iterator itend, std::string nick)
{
	while (it != itend)
	{
		if ((*it)->getNick() == nick)
			break;
		it++;
	}
	if (it != itend)
		return (it);
	return (itend);
}

void Channel::kickClient(Client &chop, Client &member, std::string reason)
{
	std::vector<Client*>::iterator find;

	if (!isMember(chop))
	{
		send_error(chop, ERR_NOTONCHANNEL, "KICK"); //You're not on that channel
		return;
	}
	if (!isOp(chop))
	{
		send_error(chop, ERR_CHANOPRIVSNEEDED, "KICK"); //You're not channel operator
		return;
	}
	if (isMember(member))
	{
		broadcastNotice(chop, *this, NTC_KICK(_channel, member.getNick(), reason));
		find = findIt(_member.begin(), _member.end(), member.getNick());
		if (find != _member.end())
			_member.erase(find);
		if (isOp(member))
		{
			find = findIt(_op.begin(), _op.end(), member.getNick());
			if (find != _op.end())
				_op.erase(find);
		}
	}
}

void	Channel::topic(Client& member, std::string topic)
{
	if (!isMember(member))
	{
		send_error(member, ERR_NOTONCHANNEL, "TOPIC"); //You're not on that channel
		return;
	}
	if (_topic_change == false && !isOp(member))
	{
		send_error(member, ERR_CHANOPRIVSNEEDED, "TOPIC"); //You're not channel operator
		return;
	}
	if (!topic.empty())
		setTopic(topic);
	if (_topic.empty())
		broadcastReply(RPL_NOTOPIC(_channel), 331); // No topic is set
	else
		broadcastReply(RPL_TOPIC(_channel, _topic), 332); // Channel :topic
}

bool Channel::broadcastMessage(Client& exclude, const std::string& message)
{
	for (size_t i = 0; i < _member.size(); i++)
	{
		Client* client = _member[i];
		if (isMember(*client) == false ||
			client->getFd() == exclude.getFd()) // TODO: Do we really this ? Only members
			continue;                           // should be  in member vector
		client->setSendBuf(message);
	}
	return true;
}

bool Channel::broadcastReply(const std::string& message, int rpl_code)
{
	std::ostringstream os;
	for (size_t i = 0; i < _member.size(); i++)
	{
		Client* client = _member[i];
		if (isMember(*client) == false) // TODO: Do we really this ? Only members
			continue;                   // should be  in member vector
		os << client->getMessageNameBase();
		if (rpl_code != 0)
			os << std::setfill('0') << std::setw(3) << rpl_code << " ";
		os << message;
		client->setSendBuf(os.str());
	}
	return true;
}

void Channel::addMode(Client& client, std::string mode, std::string argument)
{
	std::map<char, t_exe>::iterator found = _modes.find(mode[1]);

	if (!this->isMember(client))
	{
		send_error(client, ERR_NOTONCHANNEL, "MODE"); // :You're not on that channel
		return;
	}
	if (!this->isOp(client))
	{
		send_error(client, ERR_CHANOPRIVSNEEDED, "MODE"); //You're not channel operator
		return;
	}
	if (found != _modes.end() && mode.size() == 2)
		(this->*(found->second))(client, mode, argument);
}

void Channel::inviteMode(Client& client, std::string mode, std::string argument)
{
	(void)argument;
	(void)client;

	if (mode == "+i")
	{
		_invite_only = true;
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, "is now invite only."), 324);
	}
	else if (mode == "-i")
	{
		_invite_only = false;
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, "dosen't need an invite to join."), 324);
	}
}

void Channel::topicMode(Client& client, std::string mode, std::string argument)
{
	(void)argument;
	(void)client;

	if (mode == "+t")
	{
		_topic_change = true;
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, "topic change is now available to evey member."), 324);
	}
	else if (mode == "-t")
	{
		_topic_change = false;
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, "topic change is now restricted."), 324);
	}
}

void Channel::keyMode(Client& client, std::string mode, std::string argument)
{
	std::string mode_params = "";

	(void)client;
	if (mode == "+k")
	{
		if (_is_key == true)
		{
			send_error(client, ERR_KEYSET, "MODE"); // :Channel key already set
			return;
		}
		_pass = argument;
		mode_params = "set key to '" + argument + "'.";
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, mode_params), 324);
	}
	else if (mode == "-k")
	{
		_is_key = false;
		mode_params = "key '" + _pass + "' was removed.";
		_pass = "";
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, mode_params), 324);
	}
}

void Channel::addOp(Client *client)
{
	if (this->isOp(*client))
		return;
	this->_op.push_back(client);
}

void Channel::removeOp(Client *client)
{
	std::vector<Client*>::iterator find;

	find = std::find(_op.begin(), _op.end(), client);
	if (find != _op.end())
		_op.erase(find);

	if (_member.size() && _op.size() == 0)
		_op.push_back(_member[0]);
}

void Channel::operatorMode(Client& client, std::string mode, std::string argument)
{
	std::vector<Client*>::iterator find; 
	std::string mode_params = "";

	find = findIt(_member.begin(), _member.end(), argument);
	if (mode == "+o" && !this->isOp(client))
	{
		mode_params = "channel operator privileges were given to '" + argument + "'.";
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, mode_params), 324);
		addOp(*find);
	}
	else if (mode == "-o" && this->isOp(client) && _op.size() > 0)
	{
		mode_params =  "channel operator privileges were taken from '" + argument + "'.";
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, mode_params), 324);
		removeOp(*find);
	}
}

int nbrLimit(std::string str)
{
	size_t i = 0;
	int    n = -1;
	if (str.size() > 3)
		return (n);
	while (i < str.size())
	{
		if (!isdigit(str[i]))
			break;
		i++;
	}
	if (i == str.size())
		n = atoi(str.c_str());
	return (n);
}

void Channel::limitMode(Client& client, std::string mode, std::string argument)
{
	(void)client;
	std::string mode_params = "";

	if (mode == "+l")
	{
		if (argument.empty())
			argument = "10";
		//Limit number of members on the channal by default is 10.
		_is_limited = true;
		int n       = nbrLimit(argument);
		if (n == -1)
		{
			send_error(client, ERR_INVALIDMODEPARAM, "MODE");
			return ;
		}
		_limit = n;
		mode_params =  "is now limited to '" + argument + "' members.";
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, mode_params), 324);
	}
	else if (mode == "-l")
	{
		_is_limited = false;
		mode_params =  "limmit was removed.";
		broadcastReply(RPL_CHANNELMODEIS(_channel, mode, mode_params), 324);
	}
}

bool Channel::validName(const std::string& name)
{
	if (name.length() < 2 || name.length() > 200)
		return false;
	if (name.at(0) != '&' && name.at(0) != '#')
		return false;
	std::size_t s = name.find(' ');
	return s == name.npos;
}

std::vector<Client*>& Channel::getMembers()
{
	return (_member);
}

std::vector<Client*>& Channel::getOp()
{
	return (_op);
}

std::string Channel::getName()
{
	return (_channel);
}

std::string Channel::getpass()
{
	return (_pass);
}

std::string Channel::getTopic() const
{
	return (this->_topic);
}

std::string Channel::getMessageBaseName() const
{
	return ":" + std::string(SERVER_NAME) + " ";
}

void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}

bool Channel::getInviteOnly() const
{
	return _invite_only;
}

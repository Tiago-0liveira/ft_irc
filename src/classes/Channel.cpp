#include <Channel.hpp>

// std::string Channel::JOIN_MESSAGE = "The User %s joined!";
// std::string Channel::LEAVE_MESSAGE = "The User %s left!";
// std::string Channel::TOPIC_MESSAGE = "Channel %s topic is %s!";

Channel::Channel(std::string name): _channel(name), _pass(""), _topic(""), _limit(10), _op()
{
	if (_channel[0] == '#')
	{
		_is_key = true;
		_pass = "123"; // to change
	}
	else
		_is_key = false;

	_topic_change = true;
	_is_limited = false;
	_invite_only = false;

	_modes['i'] = &Channel::inviteMode;
	_modes['t'] = &Channel::topicMode;
	_modes['k'] = &Channel::keyMode;
	_modes['o'] = &Channel::operatorMode;
	_modes['l'] = &Channel::limitMode;
	
	_nbr_modes['i'] = 0;
	_nbr_modes['t'] = 0;
	_nbr_modes['k'] = 0;
	_nbr_modes['o'] = 0;
	_nbr_modes['l'] = 0;
}

Channel::~Channel()
{}

void Channel::addClient(Client &client, std::string password)
{
	std::vector<Client*>::iterator find = std::find(_member.begin(), _member.end(), &client);

	(void)password;
	if (find == _member.end())
	{
		if (_invite_only == true)
		{
			std::vector<std::string>::iterator find_invite = std::find(client.getChannalInvites().begin(), client.getChannalInvites().end(), _channel);
			if (find_invite == client.getChannalInvites().end())
			{
				std::cout << RED << client.getNick() << " dosen't have the invitation for the channel " << _channel << RESET << std::endl;
				return;
			}
		}
		if (_member.size() == _limit)
		{
			std::cout << RED << "Channel " << _channel  << " has reached max number of people." << RESET << std::endl;
			return;
		}
		_member.push_back(&client);
		//broadcastMessage(format(JOIN_MESSAGE, _member.back()->getNick()), _member.back()->getFd());
		std::cout << "The User " << _member.back()->getNick() << " joined channel " << _channel << std::endl;
	}	
	if (_op.size() == 0)
		_op.push_back(&client);
}

bool Channel::isMember(Client& client)
{
	std::vector<Client*>::iterator find;

	find = std::find(_member.begin(), _member.end(), &client);
	if (find != _member.end())
		return true;
	return false;
}

void Channel::inviteClient(Client &member, Client &invited)
{
	if (!isMember(member))
	{
		std::cout << "The User " << member.getNick() << " can't invite people/ not part of the channel " << _channel << std::endl;
		return;
	}
	std::vector<std::string>::iterator find = std::find(invited.getChannalInvites().begin(), invited.getChannalInvites().end(), _channel);
	if (find == invited.getChannalInvites().end())
	{
		invited.getChannalInvites().push_back(_channel);
		//broadcastMessage(format(JOIN_MESSAGE, _member.back()->getNick()), _member.back()->getFd());
		std::cout << "The User " << invited.getNick() << " was invided channel " << _channel << std::endl;
	}
}

bool Channel::kickClient(std::string clientNick)
{
	std::vector<Client*>::iterator find = _op.begin();

	while (find != _op.end() && clientNick.compare((*find)->getNick()))
		find++;
	if (find != _op.end())
		_op.erase(find);

	find = _member.begin();
	while (find != _member.end() && clientNick.compare((*find)->getNick()))
		find++;
	if (find == _member.end())
		return false;
	std::cout << "The User " << (*find)->getNick() << " left!" << std::endl;
	//broadcastMessage(format(LEAVE_MESSAGE, (*find)->getNick()), -1);
	_member.erase(find);

	return true;
}

void	Channel::topic(std::string topic, Client& client)
{
	if (_topic_change == false && !isOp(client))
	{
		std::cout << RED << client.getNick() << " can't change TOPIC of the channel " << _channel << RESET << std::endl;
		return;
	}
	if (!topic.empty())
		setTopic(topic);
	//sendMessage(_op[0]->getFd(), format(TOPIC_MESSAGE, _channel, _topic));
	if (_topic.empty())
		std::cout << RED << "Channel " << _channel << " dosen't have a topic." << RESET << std::endl;
	else
		std::cout << "Channel " << _channel << " topic is " << _topic << std::endl;
}

void Channel::broadcastMessage(const std::string &message, int exceptFd)
{
	std::vector<Client *>::iterator it = _member.begin();

	while (it != _member.end())
	{
		int memberFd = (*it)->getFd();
		if (memberFd != exceptFd)
		{
			sendMessage(memberFd, message);
		}	
		it++;
	}
}

void Channel::addMode(Client &client, std::string mode, std::string argument)
{
	std::map<char, t_exe>::const_iterator	found = _modes.find(mode[1]);
	std::map<char, int>::iterator aux = _nbr_modes.find(mode[1]);;
	if (aux->second >= 3)
	{
		std::cout << RED << "Error: You have already used mode " << mode[1] << " tree times " << RESET << std::endl;
		return;
	}
	if (found != _modes.end() && mode.size() == 2)
		(this->*(found->second))(client, mode, argument);
}

void Channel::inviteMode(Client &client, std::string mode, std::string argument)
{
	(void)argument;
	std::map<char, int>::iterator aux = _nbr_modes.find('i');
	
	if (!this->isOp(client))
		std::cout << RED << "Member " << client.getNick() << " cant't invete other people to the channel " << _channel << " because he/she is not an operator." << RESET << std::endl;
	else if (mode[0] == '+')
	{
		aux->second += 1;
		_invite_only = true;
		std::cout << "Mode " << mode << ": " << argument << " was invited to the channal " << _channel << std::endl;
	}
	else if (mode[0] == '-')
	{
		aux->second += 1;
		_invite_only = false;
		std::cout << "Mode " << mode << ": Invite to channel " << _channel  << " was removed from " << argument << std::endl;
	}
}

void Channel::topicMode(Client &client, std::string mode, std::string argument)
{
	(void)argument;
	std::map<char, int>::iterator aux = _nbr_modes.find('t');
	
	if (!this->isOp(client) && !_topic_change)
		std::cout << RED << "Member " << client.getNick() << " cant't change the topic in the channel " << _channel << " because he/she is not an operator." << RESET << std::endl;
	else if (mode[0] == '+')
	{
		_topic_change = true;
		aux->second += 1;
		std::cout << "Mode " << mode << ": TOPIC command on channel " << _channel  << " is free to use."  << std::endl;
	}
	else if (mode[0] == '-')
	{
		_topic_change = false;
		aux->second += 1;
		std::cout << "Mode " << mode << ": TOPIC command on channel " << _channel  << " is now restricted." << std::endl;
	}
}

void Channel::keyMode(Client &client, std::string mode, std::string argument)
{
	std::map<char, int>::iterator aux = _nbr_modes.find('k');
	
	if (!this->isOp(client))
		std::cout << RED << "Member " << client.getNick() << " dosen't have the clerence to add a password to channel " << _channel << " because he/she is not an operator." << RESET << std::endl;
	else if (mode[0] == '+')
	{
		if (_is_key == true)
		{
			std::cout << RED << "Key is already set." << _channel << std::endl;
			return;
		}
		aux->second += 1;
		_pass = argument;
		std::cout << "Mode " << mode << ":  password was add to the channal " << _channel << std::endl;
	}
	else if (mode[0] == '-')
	{
		aux->second += 1;
		_is_key = false;
		_pass = "";
		std::cout << "Mode " << mode << ": password was removed from the channel " << _channel << std::endl;
	}
}

bool Channel::isOp(Client& client)
{
	std::vector<Client*>::iterator find;

	find = std::find(_op.begin(), _op.end(), &client);
	if (find != _op.end())
		return true;
	return false;
}

void Channel::addOp(Client &client)
{
	if (this->isOp(client))
		return;
	this->_op.push_back(&client);
}

void Channel::removeOp(Client& client)
{
	std::vector<Client*>::iterator find;

	find = std::find(_op.begin(), _op.end(), &client);
	if (find != _op.end())
		_op.erase(find);
	
	if (_member.size() && _op.size() == 0)
		_op.push_back(_member[0]);
}

void Channel::operatorMode(Client &client, std::string mode, std::string argument)
{
	(void)argument;
	std::map<char, int>::iterator aux = _nbr_modes.find('o');
	
	if (mode[0] == '+' && !this->isOp(client))
	{
		std::cout << "Mode " << mode << ": " << client.getNick() << " was promoted to operator of channel " << _channel << std::endl;
		aux->second += 1;
		//broadcastMessage(str, argClient->getFd());
		addOp(client);
	}
	else if (mode[0] == '-' && this->isOp(client) && _op.size() > 0)
	{
		//broadcastMessage(str, argClient->getFd());
		std::cout << "Mode " << mode << ": " << client.getNick() << " is no longer a operator of channel " << _channel << std::endl;
		aux->second += 1;
		
		removeOp(client);
	}
}

int	nbrLimit(std::string str)
{
	size_t i = 0;
	int n = -1;
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

void Channel::limitMode(Client &client, std::string mode, std::string argument)
{
	(void)client;
	std::map<char, int>::iterator aux = _nbr_modes.find('l');;

	if (mode[0] == '+' && mode[1] == 'l')
	{
		_is_limited = true;
		int n = nbrLimit(argument);
		if (n == -1)
			std::cout << RED << "Error: invalid limit nbr on channel " << _channel << RESET << std::endl;
		_limit = n;
		aux->second += 1;
		std::cout << "Mode " << mode << ": channel " << _channel << " is now limited." << std::endl;
		//Limit number of members on the channal by default is 10.
	}
	else if (mode[0] == '-' && mode[1] == 'l')
	{
		_is_limited = false;
		aux->second += 1;
		std::cout << "Mode " << mode << ": channel " << _channel << " limmit removed." << std::endl;
	}
}

std::vector<Client *>	&Channel::getMembers()
{	return (_member);	}

std::vector<Client *>	&Channel::getOp()
{	return (_op);	}

std::map<char, int>		&Channel::getNbrMode()
{	return (_nbr_modes);	}

std::string	Channel::getName()
{	return (_channel);	}

std::string	Channel::getpass()
{	return (_pass);		}

std::string	Channel::getTopic() const
{	return (this->_topic);	}

void	Channel::setTopic(std::string topic)
{	this->_topic = topic;	}

/*
int main() 
{
	Client toy, taylor, meghan, phill, nickle;
	toy.setNick("Toy");
	taylor.setNick("Taylor");
	meghan.setNick("Meghan");
	phill.setNick("Phill");
	nickle.setNick("Nicleback");

	std::cout << GREEN << "<<<<<<<<<<<<<<<<<<<<<<<<< Add >>>>>>>>>>>>>>>>>>>>>>>>>" << RESET << std::endl;
	Channel canal("&Sing");
	canal.addClient(toy,"");
	canal.addClient(taylor,"");
	canal.addClient(meghan,"");
	canal.inviteClient(toy, phill);
	canal.addMode(meghan, "+t", "");
	canal.addMode(meghan, "+o", "");
	canal.addMode(meghan, "+l", "3");
	canal.addMode(meghan, "+k", "banana");
	canal.inviteClient(taylor, phill);
	canal.addMode(toy, "+i", "Phill");
	canal.addClient(phill,"");


	std::cout << std::endl;
	std::vector<Client*>::iterator it;
	
	std::cout << CYAN << "Members of " << canal.getName() << ": " << RESET;
	for(it = canal.getMembers().begin(); it != canal.getMembers().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl;

	std::cout << PURPLE << "Opratores of " << canal.getName() << ":  " << RESET;
	for(it = canal.getOp().begin(); it != canal.getOp().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl << std::endl;

	std::cout << GREEN << "<<<<<<<<<<<<<<<<<<<<<<<< Remove >>>>>>>>>>>>>>>>>>>>>>>>" << RESET << std::endl;
	canal.kickClient("Taylor");
	canal.addMode(toy, "-o", "remove operator");
	canal.addMode(meghan, "-t", "");
	canal.addMode(meghan, "+t", "banana");
	canal.addMode(meghan, "-t", "");
	canal.addMode(meghan, "-l", "");
	canal.addClient(phill,"");
	canal.addClient(nickle,"");

	
	std::cout << std::endl << CYAN << "Members of " << canal.getName() << ": " << RESET;
	for(it = canal.getMembers().begin(); it != canal.getMembers().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl;
	
	std::cout << PURPLE << "Opratores of " << canal.getName() << ":  " << RESET;
	for(it = canal.getOp().begin(); it != canal.getOp().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl << std::endl;

	std::cout << GREEN << "<<<<<<<<<<<<<<<<<<<<<<<< Topic >>>>>>>>>>>>>>>>>>>>>>>>" << RESET << std::endl;
	canal.topic("", taylor);
	canal.topic("poop", meghan);

	std::map<char, int>::iterator aux;
	for(aux = canal.getNbrMode().begin(); aux != canal.getNbrMode().end(); ++aux)
		std::cout << "{" << aux->first << ", " << aux->second <<  "}" << std::endl;
	std::cout << std::endl;
}
*/
#include <Channel.hpp>

std::string Channel::JOIN_MESSAGE = "The User %s joined!";
std::string Channel::LEAVE_MESSAGE = "The User %s left!";
std::string Channel::TOPIC_MESSAGE = "Channel %s topic is %s!";


Channel::Channel(std::string name): _channel(name), _pass(""), _topic(""), _op()
{
	_functions['o'] = &Channel::operatorMode;
}

Channel::~Channel()
{}

void Channel::addClient(Client &client, std::string password)
{
	std::vector<Client*>::iterator find = std::find(_member.begin(), _member.end(), &client);
	
	(void)password;
	if (find == _member.end())
	{
		_member.push_back(&client);
		//broadcastMessage(format(JOIN_MESSAGE, _member.back()->getNick()), _member.back()->getFd());
		std::cout << "The User " << _member.back()->getNick() << " joined channel " << _channel << std::endl;
	}	
	if (_op.size() == 0)
		_op.push_back(&client);
}


bool Channel::removeClient(std::string clientNick)
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
	_member.erase(find);
	//broadcastMessage(format(LEAVE_MESSAGE, (*find)->getNick()), -1);
	std::cout << "The User " << (*find)->getNick() << " left!" << std::endl;

	return true;
}

void	Channel::topic(std::string topic)
{
	if (!topic.empty())
		setTopic(topic);
	if (_op.size() > 0)
	{
		//sendMessage(_op[0]->getFd(), format(TOPIC_MESSAGE, _channel, _topic));
		std::cout << "Channel " << _channel << " topic is " << topic << std::endl;
	}
		
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

Client* Channel::findClient(std::string clientNick)
{
	std::vector<Client*>::iterator find;

	for (find = _member.begin(); find != _member.end(); ++find)
	{
		if (clientNick.compare((*find)->getNick()) == 0)
			return (*find);
	}
	return (NULL);
}


void Channel::addMode(Client &client, std::string mode, std::string argument)
{
	std::map<char, t_exe>::const_iterator	found = _functions.find(mode[1]);
	if (found != _functions.end() && mode.size() == 2)
		(this->*(found->second))(client, mode, argument);
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
	std::string str;
	if (argument.empty())
		return ;
	Client *argClient = this->findClient(argument);
	if (mode[0] == '+' && argClient && !this->isOp(*argClient))
	{
		std::cout << "Mode " << mode << " : " <<  client.getNick() << " was promoted to operator of channel " << _channel << std::endl;
		//str = format("Mode %s: %s was promoted to operator of channel %s", mode,, );
		//broadcastMessage(str, argClient->getFd());
		addOp(*argClient);
	}
	else if (mode[0] == '-' && argClient && this->isOp(*argClient) && _op.size() > 1)
	{
		str = format("Mode %s: %s is no longer a operator of channel %s", mode, client.getNick(), _channel);
		
		//broadcastMessage(str, argClient->getFd());
		removeOp(*argClient);
	}
}


std::vector<Client *>	&Channel::getMembers()
{	return (_member);	}

std::vector<Client *>	&Channel::getOp()
{	return (_op);	}

std::string	Channel::getName()
{	return (_channel);	}

std::string	Channel::getpass()
{	return (_pass);		}

std::string	Channel::getTopic() const
{	return (this->_topic);	}

void	Channel::setTopic(std::string topic)
{	this->_topic = topic;	}

std::string	Channel::getMode(char mode)
{	return _modes[(int)mode];	}

int main() 
{
	Client toy, taylor, meghan;
	toy.setNick("Toy");
	taylor.setNick("Taylor");
	meghan.setNick("Meghan");

	std::cout << GREEN << "<<<<<<<<<<<<<<<<<<<<<<<<< Add >>>>>>>>>>>>>>>>>>>>>>>>>" << RESET << std::endl;
	Channel canal("Sing");
	canal.addClient(toy,"");
	canal.addClient(taylor,"");
	canal.addClient(meghan,"");

	std::cout << std::endl;
	std::vector<Client*>::iterator it;
	
	std::cout << CYAN << "Members of " << canal.getName() << ": " << RESET;
	for(it = canal.getMembers().begin(); it != canal.getMembers().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl;

	canal.addOp(meghan);
	std::cout << PURPLE << "Opratores of " << canal.getName() << ":  " << RESET;
	for(it = canal.getOp().begin(); it != canal.getOp().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl << std::endl;


	std::cout << GREEN << "<<<<<<<<<<<<<<<<<<<<<<<< Remove >>>>>>>>>>>>>>>>>>>>>>>>" << RESET << std::endl;
	canal.removeClient("Taylor");
	std::cout << std::endl << CYAN << "Members of " << canal.getName() << ": " << RESET;
	for(it = canal.getMembers().begin(); it != canal.getMembers().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl;

	canal.removeOp(toy);
	std::cout << PURPLE << "Opratores of " << canal.getName() << ":  " << RESET;
	for(it = canal.getOp().begin(); it != canal.getOp().end(); ++it)
		std::cout << (*it)->getNick() << ", ";
	std::cout << std::endl << std::endl;

	std::cout << GREEN << "<<<<<<<<<<<<<<<<<<<<<<<< Topic >>>>>>>>>>>>>>>>>>>>>>>>" << RESET << std::endl;
	canal.topic("");
	canal.topic("famous singers");
}
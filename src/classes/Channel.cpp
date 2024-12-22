#include "errors.hpp"
#include <Channel.hpp>
#include <Commands.hpp>
#include <iomanip>
#include <sstream>

// std::string Channel::JOIN_MESSAGE = "The User %s joined!";
// std::string Channel::LEAVE_MESSAGE = "The User %s left!";
// std::string Channel::TOPIC_MESSAGE = "Channel %s topic is %s!";

const std::string Channel::DEFAULT_PASS  = "123";
const std::string Channel::DEFAULT_TOPIC = "No Topic";

Channel::Channel(std::string name, Server* server, std::string key)
    : _channel(name), _pass(key), _topic(DEFAULT_TOPIC), _limit(10), _op(), _serv_ptr(server)
{
    _serv_ptr = server;
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

    _nbr_modes['i'] = 0;
    _nbr_modes['t'] = 0;
    _nbr_modes['k'] = 0;
    _nbr_modes['o'] = 0;
    _nbr_modes['l'] = 0;
}

Channel::~Channel() {}

void Channel::addClient(Client& client, std::string password)
{
    std::vector<Client*>::iterator find = std::find(_member.begin(), _member.end(), &client);

    if (find == _member.end())
    {
        if (_invite_only)
        {
            if (client.getChannalInvites().count(_channel) == 0)
                return send_error(client, ERR_INVITEONLYCHAN, "JOIN");
        }
        if (_is_key)
        {
            if (getpass() != password)
                return send_error(client, ERR_PASSWDMISMATCH, "JOIN");
        }
        if (_member.size() == _limit)
            return send_error(client, ERR_CHANNELISFULL, "JOIN");
        if (_invite_only)
            client.getChannalInvites().erase(client.getChannalInvites().find(_channel));
        _member.push_back(&client);
        client.setSendBuf(RPL_JOIN(client.getMessageNameBase(), _channel));
        std::string topicCommandMsg = "TOPIC " + getName() + "\r\n";
        topicCommand(client, topicCommandMsg);
        broadcastMessage(client, RPL_JOIN(client.getMessageNameBase(), _channel));
        std::cout << "The User " << _member.back()->getNick() << " joined channel " << _channel
                  << std::endl;
    }
    else
        send_error(client, ERR_USERONCHANNEL, "JOIN");
    if (_op.size() == 0)
        _op.push_back(&client);
}

void Channel::removeClient(Client& client, const std::string& leave_msg)
{
    std::vector<Client*>::iterator find = std::find(_member.begin(), _member.end(), &client);
    if (find != _member.end())
    {
        broadcastMessage(client, leave_msg, false);
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
                    addOp(*cli);
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

void Channel::inviteClient(Client& member, Client& invited)
{
    if (invited.getChannalInvites().count(_channel) == 0)
    {
        invited.getChannalInvites().insert(_channel);
        send_reply(member, 341, RPL_INVITING(member.getNick(), invited.getNick(), getName()));
        invited.setSendBuf(member.getMessageNameBase() + RPL_INVITE(invited.getNick(), getName()));
        std::cout << "The User " << invited.getNick() << " was invited channel " << _channel
                  << std::endl;
    }
}

// NOTE: this function should not be for handling replies
// if your need to send a reply use the reply func: broadcastReply
bool Channel::broadcastMessage(Client& exclude, const std::string& message, bool exceptSender)
{
    for (size_t i = 0; i < _member.size(); i++)
    {
        Client* client = _member[i];
        if (exceptSender && client->getFd() == exclude.getFd())
            continue;
        client->setSendBuf(message);
    }
    return true;
}

bool Channel::broadcastReply(const std::string& message, int rpl_code)
{
    for (size_t i = 0; i < _member.size(); i++)
    {
        Client* client = _member[i];
        std::ostringstream os;
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
    std::map<char, int>::iterator   aux   = _nbr_modes.find(mode[1]);
    (void)aux;
    if (found != _modes.end())
        (this->*(found->second))(client, mode, argument);
}

void Channel::inviteMode(Client& client, std::string mode, std::string argument)
{
    (void)argument;
    std::map<char, int>::iterator aux = _nbr_modes.find('i');

    if (mode == "+i")
    {
        aux->second += 1;
        _invite_only = true;
        broadcastReply(RPL_CHANNELMODEIS(client.getNick(), getName(), mode), 324);
    }
    else if (mode == "-i")
    {
        aux->second += 1;
        _invite_only = false;
        broadcastReply(RPL_CHANNELMODEIS(client.getNick(), getName(), mode), 324);
    }
}

void Channel::topicMode(Client& client, std::string mode, std::string argument)
{
    (void)argument;
    std::map<char, int>::iterator aux = _nbr_modes.find('t');

    if (mode == "-t")
    {
        _topic_change = true;
        aux->second += 1;
        broadcastReply(RPL_CHANNELMODEIS(client.getNick(), getName(), mode), 324);
    }
    else if (mode == "+t")
    {
        _topic_change = false;
        aux->second += 1;
        broadcastReply(RPL_CHANNELMODEIS(client.getNick(), getName(), mode), 324);
    }
}

void Channel::keyMode(Client& client, std::string mode, std::string argument)
{
    std::map<char, int>::iterator aux = _nbr_modes.find('k');
    
    if (mode == "+k")
    {
        if (_is_key == true)
        {
            send_error(client, ERR_KEYSET, "MODE"); // :Channel key already set
            return;
        }
        aux->second += 1;
        broadcastReply(RPL_CHANNELMODEISARGS(client.getNick(), getName(), mode, argument), 324);
    }
    else if (mode == "-k")
    {
        aux->second += 1;
        _is_key = false;
        broadcastReply(RPL_CHANNELMODEISARGS(client.getNick(), getName(), mode, argument), 324);
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

void Channel::addOp(Client& client)
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

void Channel::operatorMode(Client& client, std::string mode, std::string argument)
{
    Server*                        ptr = client.getServer();
    std::map<char, int>::iterator  aux = _nbr_modes.find('o');
    std::vector<Client*>::iterator find;

    if (isOp(client) == false)
        return send_error(client, ERR_CHANOPRIVSNEEDED, "MODE");
    if (mode == "+o")
    {
        // mode_params = "channel operator privileges were given to '" + argument + "'.";
        aux->second += 1;
        broadcastReply(RPL_CHANNELMODEISARGS(client.getNick(), _channel, mode, argument), 324);
        addOp(*ptr->findClient(argument));
    }
    else if (mode == "-o" && _op.size() > 0)
    {
        broadcastReply(RPL_CHANNELMODEISARGS(client.getNick(), _channel, mode, argument), 324);
        aux->second += 1;
        removeOp(*ptr->findClient(argument));
    }
}

int nbrLimit(std::string str)
{
    size_t i = 0;
    int    n = -1;
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
    std::map<char, int>::iterator aux = _nbr_modes.find('l');

    if (argument.empty())
        argument = "10";
    // Limit number of members on the channal by default is 10.
    if (mode == "+l")
    {
        _is_limited = true;
        int n       = nbrLimit(argument);
        if (n == -1)
            return;
        _limit = n;
        aux->second += 1;
        broadcastReply(RPL_CHANNELMODEISARGS(client.getNick(), _channel, mode, argument), 324);
    }
    else if (mode == "-l")
    {
        _is_limited = false;
        aux->second += 1;
        broadcastReply(RPL_CHANNELMODEISARGS(client.getNick(), _channel, mode, argument), 324);
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

std::string Channel::getActiveModesInString()
{

    std::string res;

    if (_invite_only)
        res += "i";
    if (_topic_change)
        res += "t";
    if (_is_limited)
        res += "l";
    if (_is_key)
        res += "k " + getpass();

    if (!res.empty())
        res = "+" + res;

    return res;
}

std::vector<Client*>& Channel::getMembers()
{
    return (_member);
}

std::vector<Client*>& Channel::getOp()
{
    return (_op);
}

std::map<char, int>& Channel::getNbrMode()
{
    return (_nbr_modes);
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

#include <Channel.hpp>

std::string Channel::JOIN_MESSAGE = "The User %d joined!";
std::string Channel::LEAVE_MESSAGE = "The User %d left!";


Channel::Channel(unsigned int id): m_id(id) {}

Channel::~Channel() {}

Channel::Channel(const Channel &cpy): m_id(cpy.getId())
{
    *this = cpy;
}

Channel &Channel::operator=(const Channel &rhs)
{
    if (this != &rhs)
    {
        // TODO: implement = op
    }
    return *this;
}

void Channel::addClient(int fd)
{
    m_clientsFds.push_back(fd);
    broadcastMessage(format(JOIN_MESSAGE, fd), fd);
}

bool Channel::removeClient(int fd)
{
    std::vector<int>::iterator it = std::find(m_clientsFds.begin(), m_clientsFds.end(), fd);
    if (it == m_clientsFds.end())
    {
        return false;
    }
    m_clientsFds.erase(it);
    broadcastMessage(format(LEAVE_MESSAGE, fd));
    return true;
}

void Channel::broadcastMessage(const std::string &message, int exceptFd)
{
    std::vector<int>::iterator it = m_clientsFds.begin();
    while (it != m_clientsFds.end())
    {
        int clientFd = *it;
        if (exceptFd == 0 || clientFd != exceptFd)
            sendMessage(clientFd, message);
        it++;
    }
}

unsigned int Channel::getId() const
{
    return m_id;
}
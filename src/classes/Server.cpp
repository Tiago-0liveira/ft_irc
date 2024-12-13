#include <Channel.hpp>
#include <Commands.hpp>
#include <Server.hpp>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <errors.hpp>
#include <fcntl.h>
#include <iostream>
#include <irc.hpp>
#include <map>
#include <misc.hpp>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <vector>

Server::Server(int port, const std::string& password) : m_port(port), m_clients(), m_password(password), m_pollFds()
{
    int opt          = 1;
    m_name           = SERVER_NAME;
    m_dateOfCreation = "28/11/2024";
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::runtime_error("Failed to open socket");
    }

    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw std::runtime_error("Failed to set socket options");
    }
    // if (fcntl(m_socket, F_SETFL, O_NONBLOCK) == -1)
    // throw std::runtime_error(strerror(errno));

    m_address.sin_family      = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port        = htons(port);

    if (bind(m_socket, (struct sockaddr*)&m_address, sizeof(m_address)) < 0)
    {
        throw std::runtime_error(
            "Failed to bind port to server, this port might be being used by another process");
    }

    if (listen(m_socket, MAX_CLIENTS) < 0)
    {
        throw std::runtime_error("listen failed");
    }
	m_pollFds.reserve(MAX_CLIENTS + 1);
	m_clients.reserve(MAX_CLIENTS + 1);
    m_pollFds.push_back((pollfd){m_socket, POLLIN, 0});
    m_clients.push_back(Client(m_socket)); // This way the indexes are right
    m_fdNum = 1;

    m_Cmd["PASS"]    = passCommand;
    m_Cmd["USER"]    = userCommand;
    m_Cmd["NICK"]    = nickCommand;
    m_Cmd["PING"]    = pingCommand;
    m_Cmd["CAP"]     = ignoreCommand;
    m_Cmd["JOIN"]    = joinCommand;
    m_Cmd["MODE"]    = modeCommand;
    m_Cmd["WHO"]     = whoCommand;
    m_Cmd["PRIVMSG"] = privmsgCommand;
    m_Cmd["PART"]    = partCommand;
    m_Cmd["TOPIC"]   = topicCommand;
    m_Cmd["KICK"]    = kickCommand;
    m_Cmd["INVITE"]  = inviteCommand;
    // m_Cmd["NOTICE"] = noticeCommand;
    LOG(m_socket);
}

Server::~Server()
{
    std::vector<Client>::const_iterator it = m_clients.begin();

    while (it != m_clients.end())
    {
        close(it->getFd());
        it++;
    }

    close(m_socket);
}

Server::Server(const Server& cpy)
{
    *this = cpy;
}

Server& Server::operator=(const Server& rhs)
{
    if (this != &rhs)
    {
    }
    return *this;
}

void Server::start()
{
    std::cout << format("Server listening on port: %d with password: ", m_port) << m_password
              << std::endl;
    while (server_on == false)
    { /* stop if ctrl+c */
        int pollCount = poll(m_pollFds.data(), m_fdNum, -1);
        if (pollCount == -1)
            std::runtime_error("Poll error\n");
        for (int i = 0; i < m_fdNum; ++i)
        {
            if (m_pollFds[i].revents & POLLIN)
            {
                if (m_pollFds[i].fd == m_socket)
                    handleNewConnections();
                else
                    receiveData(i);
            }
            else if (m_pollFds[i].revents & POLLOUT)
            {
                Client* cli = findClient(m_pollFds[i].fd);
                if (!cli)
                    std::cerr << "Couldn't find the client to send msg to\n";
                else
                {
                    sendMessage(cli->getFd(), cli->getSendBuf());
                    cli->resetSendBuf();
                    m_pollFds[i].events |= POLLOUT;
                }
            }
            else if (m_pollFds[i].revents & POLLERR)
            {
                std::cout << "Socket error [POLLERR] " << strerror(errno);
            }
        }
        std::vector<int>::iterator add_it = m_addFds.begin();
        while (add_it != m_addFds.end())
        {
			int new_fd = *add_it;
			if (addNewFd(new_fd))
				std::cout << "new client " << new_fd << "\n";
			else
				std::cout << "new client could not connect because server is full!" << std::endl;
            add_it++;
        }
		m_addFds.clear();
        std::vector<int>::iterator it = m_deleteFds.begin();
        while (it != m_deleteFds.end())
        {
            Client* client = findClient(*it);
            if (!client)
                LOG("client is NULL for " << *it)
            else
                deleteClient(*client);
            it++;
        }
        m_deleteFds.clear();
    }
}

bool Server::addNewFd(int newfd)
{
    if (m_fdNum == MAX_CLIENTS + 1)
    {
        std::string msg = ERR_SERVERISFULL(m_name);
        sendMessage(newfd, msg);
        return false;
    }
    m_pollFds.push_back(((pollfd){newfd, POLLIN | POLLOUT | POLLERR, 0}));

    m_clients.push_back(Client(newfd));
    // Warning: Client is created but then is copied to the array so the copy constructor is called
    Client& freshClient = m_clients[m_fdNum];
    freshClient.setFd(newfd);
    freshClient.setServer(*this);

    m_fdNum++;
    return true;
}

bool Server::receiveData(int idx)
{
    char                     buf[BUFFER_SIZE];
    std::vector<std::string> splitMsg;
    Client*                  client = findClient(m_pollFds[idx].fd);

    memset(buf, 0, BUFFER_SIZE);
    int bytesRead = recv(m_pollFds[idx].fd, buf, BUFFER_SIZE, 0);
    if (bytesRead < 0)
    {
        std::cout << "Recv failure" << strerror(errno);
        m_deleteFds.push_back(m_pollFds[idx].fd);
        return false;
    }
    else if (bytesRead == 0)
    {
        std::cout << "Client disconnected\n";
        m_deleteFds.push_back(m_pollFds[idx].fd);
        return false;
    }
    else if (!client)
    {
        LOG("CLIENT IS NULL " << idx << " " << m_pollFds[idx].fd);
        return false;
    }
    client->setReadBuf(buf);
    if (client->getReadBuf().find("\r\n") != std::string::npos)
    {
        handleClientUpdates(client->getReadBuf(), *client);
        Client* client2 = findClient(m_pollFds[idx].fd);
        if (client2->getSendBuf().find("\n") != std::string::npos)
            m_pollFds[idx].events |= POLLOUT;
    }
    return true;
}

void Server::deleteClient(Client& client)
{
    std::vector<Client>::iterator it = m_clients.begin();
    int                           fd = 0;
    while (it != m_clients.end())
    {
        if (it->getNick() == client.getNick())
        {
            fd                = it->getFd();
            Client& clientRef = *it;

            std::vector<Channel>::iterator chan_it = m_channels.begin();
            while (chan_it != m_channels.end())
            {
                if (chan_it->isMember(clientRef))
                    chan_it->removeClient(clientRef);
                chan_it++;
            }

            m_clients.erase(*&it);
            break;
        }
        it++;
    }
    deleteFd(fd);
}

bool Server::deleteFd(int fd)
{
    int i = 0;
    for (; i < m_fdNum; i++)
    {
        if (m_pollFds[i].fd == fd)
            break;
    }
    m_pollFds.erase(m_pollFds.begin() + i);
    m_fdNum--;
    return true;
}
void Server::handleNewConnections()
{
    sockaddr_in clientAddr;
    socklen_t   clientAddrSize = sizeof(clientAddr);
    int         newFd          = accept(m_socket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (fcntl(newFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error(strerror(errno));
    if (newFd == -1)
        throw std::runtime_error(strerror(errno));
    m_addFds.push_back(newFd);
}

bool Server::handleClientUpdates(const std::string& input, Client& cli)
{
    std::vector<std::string>::iterator it;
    std::vector<std::string>           msg = strSplit(input, '\n');
    std::string                        command;

    // TODO: before calling any command we need to check if the client is
    // already logged in (not all commands need auth)
    // LOG("handleClientUpdates loop start\n");
    LOG(input);
    for (it = msg.begin(); it != msg.end(); it++)
    {
        std::istringstream stream(*it);
        stream >> command;
        LOG(format("%s", command.c_str()))
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        if (m_Cmd.count(command) == 1)
        {
            m_Cmd[command](cli, *it);
        }
        else
        {
            send_error(cli, ERR_UNKNOWNCOMMAND, command);
            continue;
            // return false;
        }
    }
    cli.resetReadBuf();
    return true;
}

int Server::getPort() const
{
    return m_port;
}

Client* Server::findClient(std::string const& nick)
{
    for (std::vector<Client>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        if (it->getNick() == nick)
        {
            return &(*it);
        }
    }
    return NULL;
}

Client* Server::findClient(int fd)
{
    for (std::vector<Client>::iterator it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        if (it->getFd() == fd)
        {
            return &(*it);
        }
    }
    return NULL;
}

Channel* Server::findChannel(std::string const& name)
{
    for (std::vector<Channel>::iterator it = m_channels.begin(); it != m_channels.end(); ++it)
    {
        if (it->getName() == name)
        {
            return &(*it);
        }
    }
    return NULL;
}

void Server::setHost(std::string const& host)
{
    m_host = host;
}

std::string const& Server::getHost(void) const
{
    return m_host;
}

int Server::getSocketFd() const
{
    return m_socket;
}

void Server::setName(const std::string& name)
{
    m_name = name;
}

void Server::getdateOfCreation(const std::string& date)
{
    m_dateOfCreation = date;
}

const std::string& Server::getName() const
{
    return m_name;
}

const std::string& Server::getDateOfCreation() const
{
    return m_dateOfCreation;
}

void Server::addNewChannel(Channel channel)
{
    m_channels.push_back(channel);
}

Channel* Server::getLastAddedChannel()
{
    if (m_channels.size() == 0)
        return NULL;

    return &m_channels.back();
}

bool Server::broadcastMessage(Client& cli, const std::string& message, bool exceptSender)
{
    for (size_t i = 0; i < m_channels.size(); i++)
    {
        Channel& chan = m_channels.at(0);
        chan.broadcastMessage(cli, message, exceptSender);
    }
    return true;
}

const std::string& Server::getPassword() const
{
    return m_password;
}

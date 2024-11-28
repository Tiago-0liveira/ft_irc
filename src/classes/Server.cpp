#include <Channel.hpp>
#include <Commands.hpp>
#include <Server.hpp>
#include <algorithm>
#include <cassert>
#include <cctype>
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

Server::Server(int port, const std::string& password) : m_port(port), m_password(password)
{
    int opt          = 1;
    m_name           = "mariairc";
    m_dateOfCreation = "28/11/2024";
    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::runtime_error("Failed to open socket");
    }

    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw std::runtime_error("Failed to set socket options");
    }
    if (fcntl(m_socket, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error(strerror(errno));

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
    m_pollFds.push_back((pollfd){m_socket, POLLIN, 0});
    m_clients.push_back(Client(m_socket)); // This way the indexes are right
    m_fdNum = 1;
}

Server::~Server()
{
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
    while (true)
    { /* stop if ctrl+c */
        int pollCount = poll(m_pollFds.data(), m_fdNum, -1);
        if (pollCount == -1)
            std::runtime_error("Poll error\n");
        for (int i = 0; i < m_fdNum; ++i)
        {
            if (m_pollFds[i].revents & POLLIN)
            {
                if (m_pollFds[i].fd == m_socket)
                {
                    handleNewConnections();
                    addNewFd(m_newFd);
                }
                else
                    receiveData(i);
            }
        }
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
    m_pollFds.push_back(((pollfd){newfd, POLLIN, 0}));

    m_clients.push_back(Client(m_newFd));
    // Warning: Client is created but then is copied to the array so the copy constructor is called
    Client& freshClient = m_clients[m_fdNum];
    freshClient.setFd(m_newFd);
    freshClient.setServer(*this);

    m_fdNum++;
    return true;
}

bool Server::receiveData(int idx)
{
    char                     buf[BUFFER_SIZE];
    std::vector<std::string> splitMsg;

    memset(buf, 0, BUFFER_SIZE);
    int bytesRead = recv(m_pollFds[idx].fd, buf, BUFFER_SIZE, 0);
    if (bytesRead <= 0)
    {
        std::cout << "Client disconnected\n";
        m_pollFds.erase(m_pollFds.begin() + idx);
        // m_clients.erase(client.begin() + idx);
    }
    if (bytesRead < 0)
        throw std::runtime_error(strerror(errno));
    std::string msgStr(buf);
    splitMsg = strSplit(buf, '\n');
    handleClientUpdates(splitMsg, m_clients[idx]);
    return true;
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
    return true;
}
void Server::handleNewConnections()
{
    sockaddr_in clientAddr;
    socklen_t   clientAddrSize = sizeof(clientAddr);
    m_newFd                    = accept(m_socket, (struct sockaddr*)&clientAddr, &clientAddrSize);
    if (fcntl(m_newFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error(strerror(errno));
    if (m_newFd == -1)
        throw std::runtime_error(strerror(errno));
    std::cout << "new client " << m_newFd << " from " << inet_ntoa(clientAddr.sin_addr) << ":"
              << ntohs(clientAddr.sin_port) << "\n";
    return;
}

bool Server::handleClientUpdates(std::vector<std::string>& msg, Client& cli)
{
    std::vector<std::string>::iterator it;
    std::map<std::string, FuncPtr>     m;
    std::string                        command;

    m["PASS"] = passCommand;
    m["USER"] = userCommand;
    m["NICK"] = nickCommand;
    m["PING"] = pingCommand;
    m["PONG"] = pongCommand;
    m["CAP"]  = ignoreCommand;
	m["JOIN"] = joinCommand;
    // m["PRIVMSG"] = privmsgCommand;
    // m["NOTICE"] = noticeCommand;

    for (it = msg.begin(); it != msg.end(); it++)
    {
        std::istringstream stream(*it);
        stream >> command;
		LOG(format("%s", command.c_str()))
        std::transform(command.begin(), command.end(), command.begin(), ::toupper);
        if (m.count(command) == 1)
        {
            m[command](cli, *it);
        }
        else
        {
            send_error(cli, ERR_UNKNOWNCOMMAND, command);
            return false;
        }
    }
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
	if (m_channels.size() == 0) return NULL;

    return &m_channels.back();
}

const std::string& Server::getPassword() const
{
    return m_password;
}

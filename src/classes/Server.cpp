#include <Channel.hpp>
#include <Commands.hpp>
#include <Message.hpp>
#include <Server.hpp>
#include <algorithm>
#include <cctype>
#include <errors.hpp>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <vector>

Server::Server(int port, const std::string& password) : m_port(port), m_password(password)
{
    int opt = 1;

    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::runtime_error("Failed to open socket");
    }

    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw std::runtime_error("Failed to set socket options");
    }

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
    m_pollFds.reserve(MAX_CLIENTS + 1);
    m_pollFds[0].fd     = m_socket;
    m_pollFds[0].events = POLLIN;
    m_fdNum             = 1;

    while (true)
    { /* stop if ctrl+c */
        int pollCount = poll(m_pollFds.data(), m_fdNum, -1);
        if (pollCount == -1)
            std::runtime_error("Poll error\n");
        for (int i = 1; i < m_fdNum; ++i)
        {
            if (m_pollFds[i].revents & POLLIN)
            {
                if (m_pollFds[i].fd == m_socket)
                {
                    handleNewConnections();
                    if (addNewFd(m_newFd))
                    {
                        m_clients.push_back(Client(m_newFd));
                        break;
                    }
                }
            }
            receiveData(i);
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
    m_pollFds[m_fdNum].fd     = m_newFd;
    m_pollFds[m_fdNum].events = POLLIN;
    m_fdNum++;
    return true;
}

bool Server::receiveData(int idx)
{
    char buf[BUFFER_SIZE];

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
    Message     strToMsg(msgStr);
    handleClientUpdates(strToMsg, m_clients[idx]);
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

    if (m_newFd == -1)
        throw std::runtime_error(strerror(errno));
    std::cout << "new client " << m_newFd << "from " << inet_ntoa(clientAddr.sin_addr) << ":"
              << ntohs(clientAddr.sin_port) << "\n";
}

bool Server::handleClientUpdates(Message& msg, Client& cli)
{
    std::string command(msg.getCommand());
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    std::map<std::string, FuncPtr> m;

    m["PASS"] = passCommand;
    m["USER"] = userCommand;
    m["NICK"] = nickCommand;
    m["PING"] = pingCommand;
    m["PONG"] = pongCommand;
    /* m["PRIVMSG"] = privmsgCommand;
    m["NOTICE"] = noticeCommand; */

    if (m.count(command))
    {
        m[command](cli, msg);
    }
    else
    {
        send_error(cli, ERR_UNKNOWNCOMMAND, msg.getCommand());
        return false;
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

const std::string& Server::getPassword() const
{
    return m_password;
}

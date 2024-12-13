#ifndef SERVER_HPP
#define SERVER_HPP

#include "irc.hpp"
#include "misc.hpp"
#include <Commands.hpp>
#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <set>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define MAX_CLIENTS 100
#define BUFFER_SIZE ((int)1024 * (int)2)

extern bool server_on;
class Client;
class Channel;

class Server
{
  public:
    static std::string HELP_MESSAGE;

    Server(int port, const std::string& password);
    ~Server();
    Server(const Server& cpy);
    Server& operator=(const Server& rhs);

    void start();

    int                   getPort() const;
    int                   getSocketFd() const;
    std::string const&    getHost(void) const;
    Client*               findClient(std::string const& nick);
    Client*               findClient(int fd);
    Channel*              findChannel(std::string const& name);
    void                  setHost(std::string const& host);
    const std::string&    getPassword() const;
    void                  setName(const std::string& name);
    void                  getdateOfCreation(const std::string& date);
    const std::string&    getName() const;
    const std::string&    getDateOfCreation() const;
    std::set<std::string> m_channelSet;
    void                  addNewChannel(Channel channel);
    Channel*              getLastAddedChannel();
	bool				  broadcastMessage(Client& cli, const std::string& message, bool exceptSender = true);

  private:
    void                           handleNewConnections();
    bool                           handleClientUpdates(const std::string& msg, Client& cli);
    bool                           receiveData(int index);
    void                           deleteClient(Client& client);
    bool                           addNewFd(int newfd);
    bool                           deleteFd(int fd);
    int                            m_port;
    int                            m_socket;
    int                            m_newFd;
    std::string                    m_dateOfCreation;
    int                            m_fdNum;
    std::string                    m_name;
    std::vector<Client>            m_clients;
    std::string                    m_password;
    std::string                    m_host;
    struct sockaddr_in             m_address;
    std::vector<pollfd>            m_pollFds;
    std::map<std::string, FuncPtr> m_Cmd;
    std::vector<int>               m_deleteFds;
    std::vector<Channel>           m_channels;
};

#include "Channel.hpp"
#include "Client.hpp"

#endif

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <set>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <algorithm>
#include "irc.hpp"
#include <poll.h>
#include <cstring>
#include "misc.hpp"
#include "Message.hpp"

#define MAX_CLIENTS 100
#define BUFFER_SIZE ((int)1024 * (int)2)

class Client;
class Channel;

class Server {
public:
	static std::string HELP_MESSAGE;

	Server(int port, const std::string &password);
	~Server();
	Server(const Server &cpy);
	Server &operator=(const Server &rhs);

	void start();

	int getPort() const;
    int getSocketFd() const;
    std::string const& getHost(void)const;
    Client& findClient(std::string const& nick);
    Channel& findChannel(std::string const& name);
	bool nickExists(std::string const &nick);

    void setHost(std::string const& host);
	const std::string &getPassword() const;
    std::set<std::string>m_channelSet;
    std::set<std::string>m_nickSet;

private:
	void handleNewConnections();
	bool handleClientUpdates(Message& msg, Client& cli);
    bool receiveData(int index);
    bool addNewFd(int newfd); 
    bool deleteFd(int fd);
	int m_port;
	int m_socket;
    int m_newFd;
    int m_fdNum;
    std::string m_name;
    std::vector<Client>m_clients;
	std::string m_password;
    std::string m_host;
	struct sockaddr_in m_address;
	std::vector<pollfd> m_pollFds;
	std::vector<Channel> m_channels;
};


#include "Client.hpp"
#include "Channel.hpp"

#endif

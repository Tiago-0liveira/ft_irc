#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <algorithm>
#include <irc.hpp>
#include <poll.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE ((int)1024 * (int)2)

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
	const std::string &getPassword() const;

private:
	void handleNewConnections();
	bool handleClientUpdates();

	int m_port;
	int m_socket;
	std::string m_password;
	std::map<int, std::string> m_clients;
	struct sockaddr_in m_address;
	std::vector<pollfd> m_pollFds;
};



#endif
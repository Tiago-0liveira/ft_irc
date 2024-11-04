#include <Server.hpp>

std::string Server::HELP_MESSAGE = 
	"\nList of Commands:\n"
	" - LOGIN\n"
	" - SETNICK\n"
	" - SETUSER\n"
	" - JOIN\n"
	" - QUIT\n"
	"\n";

Server::Server(int port, const std::string &password)
: m_port(port), m_password(password)
{
	int opt = 1;

    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("Failed to open socket");
    }

    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        throw std::runtime_error("Failed to set socket options");
    }

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(port);

    if (bind(m_socket, (struct sockaddr *)&m_address, sizeof(m_address)) < 0) {
        throw std::runtime_error("Failed to bind port to server, this port might be being used by another process");
    }

    if (listen(m_socket, MAX_CLIENTS) < 0) {
        throw std::runtime_error("listen failed");
    }

	m_pollFds.push_back((pollfd){ m_socket, POLLIN, 0 });
}

Server::~Server()
{
	close(m_socket);
}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

Server &Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{

	}
	return *this;
}

void Server::start()
{
	while (true) { /* stop if ctrl+c */
		int pollCount = poll(m_pollFds.data(), m_pollFds.size(), -1);
		if (pollCount == -1) {
			std::cerr << "Poll error" << std::endl;
			break;
		}
		handleNewConnections();

		for (size_t i = 1; i < m_pollFds.size(); ++i) {
			if (m_pollFds[i].revents & POLLIN) {
				int clientSocket = m_pollFds[i].fd;
				char buffer[BUFFER_SIZE];
				memset(buffer, 0, BUFFER_SIZE);

				int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
				if (bytesRead <= 0) {
					std::cout << "Client disconnected fd: " << clientSocket << std::endl;
					close(clientSocket);
					m_pollFds.erase(m_pollFds.begin() + i);
					m_clients.erase(clientSocket);
					--i;
				} else {
					m_clients[clientSocket] += std::string(buffer, bytesRead);

					// Process message with basic command handling
					std::string& message = m_clients[clientSocket];
					if (message.find("\n") != std::string::npos) {
						std::string command = message.substr(0, message.find("\n"));
						message.erase(0, message.find("\n") + 1);

						if (command == "HELP") {
							sendMessage(clientSocket, HELP_MESSAGE);
						} else if (command == "QUIT") {
							close(clientSocket);
							m_pollFds.erase(m_pollFds.begin() + i);
							m_clients.erase(clientSocket);
							--i;
						} else {
							sendMessage(clientSocket, "Unknown command\n");
						}
					}
				}
			}
		}
	}
}

void Server::handleNewConnections()
{
	if (m_pollFds[0].revents & POLLIN) {
		sockaddr_in clientAddr;
		socklen_t clientAddrSize = sizeof(clientAddr);
		int clientSocket = accept(m_socket, (struct sockaddr*)&clientAddr, &clientAddrSize);

		if (clientSocket == -1) {
			std::cerr << "Failed to accept client" << std::endl;
			return;
		}

		// Add new client to poll list (no need to set non-blocking mode)
		m_pollFds.push_back((pollfd){clientSocket, POLLIN | POLLOUT, 0});
		m_clients[clientSocket] = "";  // Initialize client buffer
		std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << " fd: " << clientSocket << std::endl;
	} 
}

bool Server::handleClientUpdates()
{
	return false;
}

int Server::getPort() const
{
	return m_port;
}

int Server::getSocketFd() const
{
	return m_socket;
}

const std::string &Server::getPassword() const
{
	return m_password;
}


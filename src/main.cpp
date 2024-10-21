#include <iostream>

#include <cstdlib>
#include <climits>
#include <cerrno>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>


#define PORT 8080
#define MAX_CLIENTS 10

int create_server_socket(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cout << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cout << "setsockopt" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Bind socket to the address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cout << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        std::cout << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

int parse_port(char *str, bool &error)
{
	long n = atol(str);
	if (errno == ERANGE || n > INT_MAX || n < INT_MIN)
	{
		error = true;
		return 0;
	}
	return static_cast<int>(n);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}


	LOG("ola")
	bool err = false;
	int port = parse_port(argv[1], err);
	if (err)
	{
		std::cerr << "Invalid port: " << argv[1] << std::endl;
		return 1;
	}
	const std::string password(argv[1]);

	int server_fd = create_server_socket(port);

	std::cout << "Server started on port " << port << std::endl;
	std::vector<struct pollfd> fds;

    // Add the server socket to the poll list
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;  // Ready to read incoming connection
    fds.push_back(server_pollfd);

    while (true) {
        int poll_count = poll(fds.data(), fds.size(), -1);  // Wait indefinitely
        if (poll_count == -1) {
            std::cout << "poll" << std::endl;
            break;
        }

        // Check if there's a new connection
        if (fds[0].revents & POLLIN) {
            int new_socket;
            struct sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, &client_len)) < 0) {
                std::cout << "accept" << std::endl;;
                continue;
            }

            // Add new client to the poll list
            struct pollfd new_pollfd;
            new_pollfd.fd = new_socket;
            new_pollfd.events = POLLIN;
            fds.push_back(new_pollfd);

            std::cout << "New client connected, socket fd: " << new_socket << std::endl;
        }

        // Handle communication for each connected client
        for (size_t i = 1; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                char buffer[1024] = {0};
                int valread = recv(fds[i].fd, buffer, 1024, 0);
                if (valread <= 0) {
                    // Client disconnected or error
                    close(fds[i].fd);
                    fds.erase(fds.begin() + i);
                    std::cout << "Client disconnected" << std::endl;
                    --i;  // Adjust the index after removing the client
                } else {
                    // Handle incoming data (websocket handshake, messages, etc.)
                    std::cout << "Received data: " << buffer << std::endl;
                    // Process WebSocket frames here
                }
            }
        }
    }

    close(server_fd);

	return 0;
}
#include <iostream>

#include <cstdlib>
#include <climits>
#include <cerrno>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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

	bool err = false;
	int port = parse_port(argv[1], err);
	if (err)
	{
		std::cerr << "Invalid port: " << argv[1] << std::endl;
		return 1;
	}
	const std::string password(argv[1]);

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Failed to set socket options" << std::endl;
        close(sockfd);
        return 1;
    }

	// Setup address
    const sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = {
			.s_addr = inet_addr("127.0.0.1")
		},
		.sin_zero = {0,0,0,0,0,0,0,0}
	};
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(sockaddr_in)) < 0)
	{
		std::cerr << "Binding socket to port failed" << std::endl;
        close(sockfd);
        return 1;
	}

	if (listen(sockfd, 3) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        close(sockfd);
        return 1;
    }

	std::cout << "Server is listening on port " << port << std::endl;

	int new_socket, addr_len = 0;
    while (true) {
        new_socket = accept(sockfd, (struct sockaddr*)&server_addr, (socklen_t*)&addr_len);
        if (new_socket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            close(sockfd);
            return 1;
        }

        std::cout << "Connection accepted from " << inet_ntoa(server_addr.sin_addr) 
                  << ":" << ntohs(server_addr.sin_port) << std::endl;

        // Close the client socket after handling
        close(new_socket);
    }


	close(sockfd);
	return 0;
}
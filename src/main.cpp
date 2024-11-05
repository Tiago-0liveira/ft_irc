#include <iostream>

#include "../include/Server.hpp"


// Entry point of the IRC server
int main(int argc, char* argv[]) {
    // Ensure proper usage: ./ircserv <port> <password>
    if (argc != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        return 1;
    }

	bool parse_port_error = true;
    int port = parse_port(argv[1], parse_port_error);
	if (!parse_port_error)
	{
		std::cerr << "Invalid port: " << argv[1] << std::endl;
		exit(1);
	}
    std::string password = argv[2];
	
	Server sv(port, password);

	sv.start();

    return 0;
}

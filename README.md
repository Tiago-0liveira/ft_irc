# ft_irc

## Overview

**ft_irc** is an IRC (Internet Relay Chat) server implementation written in C++. It is designed to handle multiple clients and channels, supporting a range of IRC commands and server functionalities. This project aims to give a practical understanding of network programming, socket handling, and the IRC protocol.

## Features

- Handles multiple clients simultaneously
- Channel creation and management
- Standard IRC commands: NICK, USER, JOIN, PART, QUIT, PRIVMSG, KICK, INVITE, MODE, TOPIC, WHO, etc.
- User and channel modes
- Message of the Day (MOTD) and server notices
- Invite and operator privileges
- Error and reply messages according to the IRC protocol

## Tech Stack

- **Language:** C++ (C++98 standard)
- **Sockets:** POSIX sockets (Linux)
- **Build System:** Makefile

## Getting Started

### Prerequisites

- A Linux environment (tested on Unix-like systems)
- G++ or Clang
- GNU `make` 

### Build Instructions

1. **Clone the repository:**
    ```bash
    git clone https://github.com/Tiago-0liveira/ft_irc.git
    cd ft_irc
    ```

2. **Build the server:**
    ```bash
    make
    ```

    This will generate the `ircserv` executable in your directory.

### Running the Server

To run the IRC server, use:

```bash
./ircserv <port> <password>
```

- `<port>`: Port number for the server to listen on (e.g., 6667)
- `<password>`: Server password required for clients to connect

Example:

```bash
./ircserv 6667 mysecret
```

### Connecting as a Client

You can use any IRC client (e.g., irssi, weechat, HexChat, or netcat for manual testing):

```bash
irssi -c 127.0.0.1 -p 6667 -w mysecret
```

## Basic IRC Commands Supported

- `/nick <nickname>`: Set or change your nickname
- `/user <username> <hostname> <servername> <realname>`: Register your user
- `/join <#channel>`: Join or create a channel
- `/part <#channel>`: Leave a channel
- `/privmsg <target> <message>`: Send private message to user or channel
- `/quit [message]`: Disconnect from the server
- `/invite <user> <#channel>`: Invite a user to a channel
- `/kick <#channel> <user> [reason]`: Remove a user from a channel
- `/mode <target> [modes]`: Change user or channel modes
- `/topic <#channel> <topic>`: Set channel topic
- `/who [mask]`: List users

## Project Structure

- `src/` - Source files for server, client, and command handling
- `include/` - Header files for core classes
- `Makefile` - Build instructions

## Configuration

You can adjust server parameters and modes in `include/misc.hpp` and `include/Server.hpp` (e.g., maximum clients, buffer sizes, available modes).

## License

This project is licensed under [The Unlicense](./LICENSE).

## Authors

- [Tiago-0liveira](https://github.com/Tiago-0liveira)
- [adilsxn](https://github.com/adilsxn)

---

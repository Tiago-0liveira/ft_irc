#ifndef CHANNEL_H
#define CHANNEL_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <misc.hpp>

class Channel
{
public:
    static std::string JOIN_MESSAGE;
    static std::string LEAVE_MESSAGE;

    Channel(unsigned int id);
    ~Channel();
    Channel(const Channel &cpy);
    Channel &operator=(const Channel &rhs);

    void addClient(int fd);
    /* 
        @return
            - 1 if fd exists and removes it from the channel sending a message to all others saying he left

            - 0 if fd does not exist
     */
    bool removeClient(int fd);
    /* 
        @param message: the message that will be broadcasted
        @param exceptFd: if different than 0 will not broadcast message to that fd
     */
    void broadcastMessage(const std::string &message, int exceptFd = 0);

    unsigned int getId() const;

private:
    // TODO: change to class Client (for now we're using int)
    std::vector<int> m_clientsFds;
    const unsigned int m_id;
};

#endif
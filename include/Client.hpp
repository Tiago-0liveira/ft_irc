#ifndef CLIENT
#define CLIENT

#pragma once

#include <ctime>
#include <string>
#include <vector>

class Channel;
class Server;

#define NICK_MIN_LENGTH 4
#define NICK_MAX_LENGTH 9

class Client
{
  public:
    Client(void);
    Client(int fd);
    Client(const Client& src);
    Client& operator=(const Client& rhs);
    // NOTE: std::find requires to define a way to compare user defined types
    // thus I provided this function in order for client to be used with
    // std::find
    bool operator==(const Client* rhs) const;
    bool operator!=(const Client& rhs) const;
    ~Client(void);

    // friend class Channel;

    int                      getFd(void) const;
    const std::string&       getHost(void) const;
    const std::string&       getNick(void) const;
    const std::string&       getUser(void) const;
    const std::string&       getServname(void) const;
    const std::string&       getRealname(void) const;
    const std::string&       getMode(void) const;
    const std::string&       getPass(void) const;
    const std::string&       getReadBuf(void) const;
    const std::string&       getSendBuf(void) const;
	std::string				 getMessageNameBase(void) const;
    time_t                   lastActiveWhen(void) const;
    bool                     getStatus(void) const;
    bool                     isAuth(void) const;
    bool                     isReg(void) const;
    Server*                  getServer(void) const;
    std::vector<std::string> getChannalInvites(void) const;

    void setFd(int fd);
    void setHost(std::string const& host);
    void setSendBuf(const std::string& msg);
    void setReadBuf(const std::string& msg);
    void resetReadBuf(void);
    void resetSendBuf(void);
    void setNick(std::string const& nick);
    void setUser(std::string const& user);
    void setRealname(std::string const& realname);
    void setServname(std::string const& servname);
    void setMode(std::string const& mode);
    void setReg(void);
    void setAuth(void);
    void setPass(std::string const& pass);
    bool isPasswordSet(void) const;
    void setLastActive(time_t when);
    void setStatus(void);
    void setServer(Server& serv);
	static bool validNick(const std::string& nick);

  private:
    int                      _fd;
    std::string              _hostname, _realname, _nickname, _username, _mode;
    std::string              _passwd;
    std::string              _servername;
    std::string              _sendbuf;
    std::string              _readbuf;
    std::vector<std::string> _channel_invites;
    Server*                  _serv;
    time_t                   _lastActive;
    bool                     _auth;
    bool                     _reg;
    bool                     _pSet;
    bool                     _status;
};

#include "Channel.hpp"
#include "Server.hpp"

#endif // !CLIENT

#ifndef IRC_HPP
#define IRC_HPP

#include <ctime>
#include <iostream>

#include <cstdlib>
#include <climits>
#include <cerrno>

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <algorithm>

#define LOG(x) std::cout << "LOG::" <<  __FILE__ << ":" << __LINE__ << "|" << x << std::endl;

class Server {

};

class Client {

};

class Channel {
    
};


class User {
    public:
        User(void);
        User(std::string& nick, std::string& user, int fd);
        User(const User& src);
        User& operator=(const User& rhs);
        ~User(void);
        const int&         getFd(void)const;
        const std::string& getHost(void)const;
        const std::string& getNick(void)const;
        const std::string& getUser(void)const;
        const std::string& getMode(void)const;
        const std::string& getPass(void)const;
        const std::string& getServ(void)const;
        const time_t* lastActiveWhen(void);
        const bool    getStatus(void)const;
        const bool    isAuth(void)const;
        const bool    isUserd(void)const;
        const bool    isOper(void)const;
        const Channel* getCurrChan(void)const;

        void setFd(int fd);
        void setHost(std::string& host);
        void setNick(std::string& nick);
        void setUser(std::string& user);
        void setMode(std::string& mode);
        void setPass(std::string& pass);
        void setServ(std::string& serv);
        void setLastActive(time_t& when);
        void setStatus(void);
        void setIsAuth(void);
        void setIsUser(void);
        void setIsOper(void);
        void setCurrChan(Channel& curr);
    private:
        int _fd;
        std::string _hostname;
        std::string _nick;
        std::string _user; 
        std::string _mode;
        std::string _passwd;
        std::string _servername;
        time_t      *_lastActive;
        bool        _status;
        bool        _isAuth;
        bool        _isUserd;
        bool        _isOper;
        Channel     *_currChan;
};

struct Message {
    std::string content;
};






#endif

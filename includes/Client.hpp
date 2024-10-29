#ifndef CLIENT
#define CLIENT

#pragma once 
#include <string>
class Channel;

class Client {
    public:
        Client(void);
        Client(std::string& nick, std::string& user, int fd);
        Client(const Client& src);
        Client& operator=(const Client& rhs);
        ~Client(void);
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
        std::string _realname;
        std::string _nickname;
        std::string _username; 
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

#endif // !CLIENT

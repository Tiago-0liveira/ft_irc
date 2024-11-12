#ifndef CLIENT
#define CLIENT

#pragma once 

#include <string>
class Channel;
class Server;

class Client {
    public:
        Client(void);
        Client(int fd);
        Client(const Client& src);
        Client& operator=(const Client& rhs);
        ~Client(void);

        //friend class Channel;

        const int&         getFd(void)const;
        const std::string& getHost(void)const;
        const std::string& getNick(void)const;
        const std::string& getUser(void)const;
        const std::string& getMode(void)const;
        const std::string& getPass(void)const;
        const std::string& getServ(void)const;
        const time_t* lastActiveWhen(void);
        bool    getStatus(void)const;
        bool    isAuth(void)const;
        bool    isReg(void)const;
        bool    isOper(void)const;
        const Channel* getCurrChan(void)const;
        const Server*  getServer(void)const;

        void setFd(int fd);
        void setHost(std::string const& host);
        void setNick(std::string const& nick);
        void setUser(std::string const& user);
        void setMode(std::string const& mode);
        void setReg(void);
        void setAuth(void);
        void setPass(std::string const& pass);
        void setServ(std::string const& serv);
        void setLastActive(time_t& when);
        void setStatus(void);
        void setIsOper(void);
        void setCurrChan(Channel & curr);
        void setServer(Server & serv);

    private:

        int _fd;
        std::string _hostname, _realname, _nickname, _username, _mode;
        std::string _passwd;
        std::string _servername;
        Server      *_serv;
        time_t      *_lastActive;
        bool        _auth;
        bool        _reg;
        bool        _status;
        bool        _isOper;
        Channel     *_currChan;
};

#endif // !CLIENT

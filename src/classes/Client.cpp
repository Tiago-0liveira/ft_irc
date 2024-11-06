#include "../../include/Client.hpp"

//TODO: add all the implem of the client class


Client::Client(void){
    return ;
}
Client::Client(int fd)
:_fd(fd)
{
    return ;
}

Client::Client(const Client& src){
    return ;
}

Client& Client::operator=(const Client& rhs){
    return *this;
}

Client::~Client(void){
    return ;
}

const int&         Client::getFd(void)const{
    return _fd;
}

const std::string& Client::getHost(void)const{
    return _hostname;
}

const std::string& Client::getNick(void)const{ 
    return _nickname;
}

const std::string& Client::getUser(void)const{
    return _username;
}

const std::string& Client::getMode(void)const{ 
    return _mode;
}

const std::string& Client::getPass(void)const{
    return _passwd;
}

const std::string& Client::getServ(void)const{
    return _servername;
}

const time_t* Client::lastActiveWhen(void){
    return _lastActive;
}

const bool    Client::getStatus(void)const{
    return _status;
}

const bool    Client::isAuth(void)const{
    return _auth;
}

const bool    Client::isReg(void)const{
    return _reg;
}

const bool    Client::isOper(void)const{
    return _isOper;
}

const Channel* Client::getCurrChan(void)const{
    return _currChan;
}

const Server* Client::getServer(void)const{
    return _serv;
}

void Client::setFd(int fd){
    _fd = fd;
}

void Client::setHost(std::string const& host){
    _hostname = host;
}

void Client::setNick(std::string const& nick){
    _nickname = nick;
}

void Client::setUser(std::string const& user){
    _username = user;
}

void Client::setMode(std::string const& mode){
    _mode = mode; 
}

void Client::setPass(std::string const& pass){
    _passwd = pass; 
}

void Client::setServ(std::string const& serv){
    _servername = serv;
}

void Client::setLastActive(time_t& when){
    _lastActive = &when;
}

void Client::setStatus(void){
    _status = true;
}

void Client::setAuth(void){
    _auth = true;
}
void Client::setReg(void){
    _reg = true;
}
void Client::setIsOper(void){
    _isOper = true;
}
void Client::setCurrChan(Channel& curr){
    _currChan = &curr;
}

void Client::setServer(Server& serv){
    _serv = &serv;
}


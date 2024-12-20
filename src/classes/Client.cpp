#include <Client.hpp>
#include <errors.hpp>
#include <stdexcept>

// TODO: add all the implem of the client class

Client::Client(void)
{
    return;
}

Client::Client(int fd) : _fd(fd), _hostname(LOCALHOST)
{
    _auth = false;
    _reg  = false;
    _pSet = false;
    std::cout << "Client constructor _fd:" << _fd << std::endl;
    return;
}

Client::Client(const Client& src)
{
    *this = src;
    return;
}

Client& Client::operator=(const Client& rhs)
{
    if (this != &rhs)
    {
        _hostname        = rhs.getHost();
        _realname        = rhs.getRealname();
        _nickname        = rhs.getNick();
        _username        = rhs.getUser();
        _mode            = rhs.getMode();
        _passwd          = rhs.getPass();
        _servername      = rhs.getServname();
        _channel_invites = rhs.getChannalInvites();
        _serv            = rhs.getServer();
        _lastActive      = rhs.lastActiveWhen();
        _auth            = rhs.isAuth();
        _reg             = rhs.isReg();
        _pSet            = rhs.isPasswordSet();
        _status          = rhs.getStatus();
        _fd              = rhs.getFd();
        _readbuf         = rhs.getReadBuf();
        _sendbuf         = rhs.getSendBuf();
    }
    return *this;
}

bool Client::operator==(const Client* rhs) const
{
    return _nickname == rhs->getNick();
}
bool Client::operator!=(const Client& rhs) const
{
    return _nickname != rhs.getNick();
}

Client::~Client(void)
{
    return;
}

int Client::getFd(void) const
{
    return _fd;
}

const std::string& Client::getHost(void) const
{
    return _hostname;
}

const std::string& Client::getNick(void) const
{
    return _nickname;
}

const std::string& Client::getUser(void) const
{
    return _username;
}

const std::string& Client::getRealname(void) const
{
    return _realname;
}

const std::string& Client::getMode(void) const
{
    return _mode;
}

const std::string& Client::getPass(void) const
{
    return _passwd;
}

const std::string& Client::getServname(void) const
{
    return _servername;
}

time_t Client::lastActiveWhen(void) const
{
    return _lastActive;
}

bool Client::getStatus(void) const
{
    return _status;
}

bool Client::isAuth(void) const
{
    return _auth;
}

bool Client::isReg(void) const
{
    return _reg;
}

Server* Client::getServer(void) const
{
    return _serv;
}

std::vector<std::string> Client::getChannalInvites(void) const
{
    return _channel_invites;
}

const std::string& Client::getReadBuf(void) const
{
    return _readbuf;
}

const std::string& Client::getSendBuf(void) const
{
    return _sendbuf;
}

const std::string& Client::getLeaveMsg(void) const
{
    return _leavemsg;
}

std::string Client::getMessageNameBase(void) const
{
    return ":" + getNick() + "!" + getUser() + "@" + getHost() + " ";
}

void Client::setFd(int fd)
{
    _fd = fd;
}

void Client::setHost(std::string const& host)
{
    _hostname = host;
}

void Client::setNick(std::string const& nick)
{
    _nickname = nick;
}

void Client::setUser(std::string const& user)
{
    _username = user;
}

void Client::setRealname(std::string const& realname)
{
    _realname = realname;
}

void Client::setServname(std::string const& servername)
{
    _username = servername;
}

void Client::setMode(std::string const& mode)
{
    _mode = mode;
}

void Client::setPass(std::string const& pass)
{
    _passwd = pass;
    _pSet   = true;
}

bool Client::isPasswordSet(void) const
{
    return _pSet;
}

void Client::setLastActive(time_t when)
{
    _lastActive = when;
}

void Client::setStatus(void)
{
    _status = true;
}

void Client::setAuth(void)
{
    _auth = true;
}

void Client::setReg(void)
{
    _reg = true;
}

void Client::setServer(Server& serv)
{
    _serv = &serv;
}

void Client::setLeaveMsg(std::string leavemsg)
{
	_leavemsg = leavemsg;
}

bool Client::validNick(const std::string& nick)
{
    if (nick.empty() || nick.length() < NICK_MIN_LENGTH || nick.length() > NICK_MAX_LENGTH)
        return false;

    char firstChar = nick[0];
    if (!std::isalpha(firstChar) && firstChar != '_' && firstChar != '^')
        return false;
    for (unsigned long i = 0; i < nick.size(); i++)
	{
		char c = nick.at(i);
        if (!std::isalnum(c) && c != '-' && c != '[' && c != ']' && c != '\'' && c != '^' && c != '_' && c != '{' && c != '}' && c != '|')
	        return false;
	}
    return true;
}

void Client::setSendBuf(const std::string& msg)
{
    _sendbuf += msg;
}

void Client::setReadBuf(const std::string& msg)
{
    _readbuf += msg;
}

void Client::resetReadBuf(void)
{
    _readbuf.clear();
}

void Client::resetSendBuf(void)
{
    _sendbuf.clear();
}

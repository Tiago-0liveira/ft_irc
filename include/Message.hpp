#ifndef MESSAGE
#define MESSAGE

#include <deque>
#include <string>

#define MSG_PRF ":"
#define MSG_END "\r\n"

class Message {
public:
    Message(std::string& input);
    Message(const Message & src);
    Message &operator=(const Message & rhs); 
    ~Message();
    void setPrefix(std::string const& prefix);
    void setCommand(std::string const& command);
    void setArgs(std::deque<std::string> const& args);
    std::string const& getPrefix(void)const;
    std::string const& getCommand(void)const;
    std::deque<std::string> const &getArgs(void) const;
    // std::deque<std::string> const& getArgs(void)const;
    std::string _target, _prefix, _command, _singleArg;
    std::deque<std::string>_targets, _args;
    //PRIVMSG maria, adilson : "Ola tudo bem"
};

#endif // !MESSAGE

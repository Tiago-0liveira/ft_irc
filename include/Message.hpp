#ifndef MESSAGE
#define MESSAGE

#include <deque>
#include <string>

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
    std::deque<std::string> const& getArgs(void)const;
private:
    std::string _prefix, _command;
    std::deque<std::string>_args;
};

#endif // !MESSAGE
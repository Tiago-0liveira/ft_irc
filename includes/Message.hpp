#ifndef MESSAGE
#define MESSAGE

#include <string>
#include <vector>

class Message {
public:
    Message(std::string& input);
    Message(const Message & src);
    Message &operator=(const Message & rhs); 
    ~Message();

private:
    std::string _prefix, _command, _args;
    // std::vector<std::string>_params;
};

#endif // !MESSAGE

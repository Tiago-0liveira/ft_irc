#include "../includes/Message.hpp"
#include <sstream>
#include <string>
#include <vector>

//TODO: parser for generating messages types to feed specific
//commands parsers 

//basic string splitter
std::vector<std::string>strSplit(std::string const&s, char delim){
    std::vector<std::string> res;
    std::stringstream ss(s);
    std::string split;
    while(getline(ss, split, delim)){
        res.emplace_back(split);
    }
    return res;
}

Message::Message(std::string& input){
    std::vector<std::string> lines;
    if (input.find(":") == std::string::npos)
        lines = strSplit(input, ':');
    if (lines[0].size() == 0)
        lines.erase(lines.begin());
    std::vector<std::string>args = strSplit(lines[0], ' ');
    _prefix = args[0];
    _command = args[0]; 
}

Message::Message(const Message & src){
    return ;
}

Message & Message::operator=(const Message & rhs){
    return *this;
} 

Message::~Message(){
    return ;
}

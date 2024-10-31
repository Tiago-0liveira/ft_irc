#include "../includes/Message.hpp"
#include <sstream>
#include <string>
#include <deque>

//TODO: parser for generating messages types to feed specific
//command parsers 

//basic string splitter
std::deque<std::string>strSplit(std::string const&s, char delim){
    std::deque<std::string> res;
    std::stringstream ss(s);
    std::string split;
    while(getline(ss, split, delim)){
        res.emplace_back(split);
    }
    return res;
}

Message::Message(std::string& input){
    std::deque<std::string> prefixCommandsArgs, lines, args;
    std::string prefix, command;
    //Split the message by ':'
    lines = strSplit(input, ':');
    //if the 1st string is empty it means the msg starts with ':'
    //thus it includes prefix
    if (lines[0].size() == 0){
        prefixCommandsArgs = strSplit(lines[1], ' ');
        _prefix = prefixCommandsArgs.front();
        prefixCommandsArgs.pop_front();
        _command = prefixCommandsArgs.front();
        prefixCommandsArgs.pop_front();
        _args = prefixCommandsArgs;
    }
    //no prefix case
    else {
        prefixCommandsArgs = strSplit(lines[1], ' ');
        _command =  prefixCommandsArgs.front();
        prefixCommandsArgs.pop_front();
        _args = prefixCommandsArgs;
    }
}

Message::Message(const Message & src){
    (void)src;
    return ;
}

Message & Message::operator=(const Message & rhs){
    (void)rhs;
    return *this;
} 

Message::~Message(){
    return ;
}


void Message::setPrefix(std::string const& prefix){
    _prefix = prefix;
}

void Message::setCommand(std::string const & command){
    _command = command;
}

void Message::setArgs(std::deque<std::string> const& args){
    _args = args;
}

std::string const& Message::getPrefix(void)const{
    return _prefix;
}

std::string const& Message::getCommand(void)const{
    return _command;
}

std::deque<std::string> const& Message::getArgs(void)const{
    return _args;
}


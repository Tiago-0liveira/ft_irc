#include "../include/Message.hpp"
#include <sstream>
#include <string>
#include <deque>

//TODO: parser for generating messages types to feed specific
//command parsers 

std::string trimSpace(std::string const& input)
{
    const char* s = " \t\n\r\f\v";
    int end  =  input.find_last_not_of(s);
    int start =  input.find_first_not_of(s);
    if (start == end)
        return input;
    return input.substr(start, end - start + 1);
}

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

    std::string cleanStr = trimSpace(input);

    //Split the message by ':'
    lines = strSplit(cleanStr, ':');
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


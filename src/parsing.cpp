#include <string>
#include <vector>
#include <sstream>
#include "misc.hpp"



std::string trimSpace(std::string const& input)
{
    const char* s     = " \t\n\r\f\v";
    int         end   = input.find_last_not_of(s);
    int         start = input.find_first_not_of(s);
    if (start == end)
        return input;
    return input.substr(start, end - start + 1);
}

std::vector<std::string> strSplit(std::string const& s, char delim)
{
    std::vector<std::string> res;
    std::stringstream       ss(s);
    std::string             split;
    while (getline(ss, split, delim))
    {
        res.push_back(split);
    }
    return res;
}




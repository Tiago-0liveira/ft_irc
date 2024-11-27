#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/errors.hpp"
#include <sstream>
#include <string>


/* 
	This function was created to ignore some command we may recieve
 */
void ignoreCommand(Client& cli, std::string& msg)
{
	(void)cli;
	(void)msg;
}



#include "../../include/Client.hpp"
#include "../../include/Commands.hpp"
#include "../../include/Message.hpp"
#include "../../include/errors.hpp"

/*  Command: USER
   Parameters: <username> <hostname> <servername> <realname>

   The USER message is used at the beginning of connection to specify
   the username, hostname, servername and realname of s new user.  It is
   also used in communication between servers to indicate new user
   arriving on IRC, since only after both USER and NICK have been
   received from a client does a user become registered.

   Between servers USER must to be prefixed with client's NICKname.
   Note that hostname and servername are normally ignored by the IRC
   server when the USER command comes from a directly connected client
   (for security reasons), but they are used in server to server
   communication.  This means that a NICK must always be sent to a
   remote server when a new user is being introduced to the rest of the
   network before the accompanying USER is sent.

   It must be noted that realname parameter must be the last parameter,
   because it may contain space characters and must be prefixed with a
   colon (':') to make sure this is recognised as such.

   Since it is easy for a client to lie about its username by relying
   solely on the USER message, the use of an "Identity Server" is
   recommended.  If the host which a user connects from has such a
   server enabled the username is set to that as in the reply from the
   "Identity Server".

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED

   Examples:


   USER guest tolmoon tolsun :Ronnie Reagan

                                   ; User registering themselves with a
                                   username of "guest" and real name
                                   "Ronnie Reagan".


   :testnick USER guest tolmoon tolsun :Ronnie Reagan
                                   ; message between servers with the
                                   nickname for which the USER command
                                   belongs to
   */

void userCommand(Client& cli, Message& msg)
{
    if (msg._args.empty() || msg._args.size() < 4)
    {
        send_error(cli, ERR_NEEDMOREPARAMS, msg.getCommand());
        return;
    }
    else if (cli.isAuth())
    {
        send_error(cli, ERR_ALREADYREGISTRED, msg.getCommand());
        return;
    }
    cli.setUser(msg._args[0]);
    cli.setHost(msg._args[1]);
    cli.setServname(msg._args[2]);
    cli.setRealname(msg._args[3] + msg._args[4]);
    cli.setReg();
    if (!cli.isAuth() && !cli.isPasswordSet())
        return;
    std::cout << "User #" << cli.getFd() << " registered as " << cli.getNick();
    // TODO: MesssageOfTheDay func()
    return;
}

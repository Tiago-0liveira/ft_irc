#include <misc.hpp>

int parse_port(char *str, bool &error)
{
	long n = atol(str);
	if (errno == ERANGE || n > INT_MAX || n < INT_MIN)
	{
		error = true;
		return 0;
	}
	return static_cast<int>(n);
}

std::string format(const std::string messageFormat, ...)
{
    const int bufferSize = 1024;
    char buffer[bufferSize];

    va_list args;
    va_start(args, messageFormat);

    std::vsnprintf(buffer, bufferSize, messageFormat.c_str(), args);

    va_end(args);

    return std::string(buffer);
}

void sendMessage(int clientSocket, const std::string& message)
{
    send(clientSocket, message.c_str(), message.length(), 0);
}

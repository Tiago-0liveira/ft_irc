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
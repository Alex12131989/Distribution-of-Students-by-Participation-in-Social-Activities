#include "Exception.h"

Exception::Exception(const std::string& message) : runtime_error(message)
{
	if (message == "Not allowed character in password")
		code = 1;
	else if (message == "Not allowed character in name")
		code = 2;
	else if (message == "Name was not entered")
		code = 3;
	else if (message == "Password was not entered")
		code = 4;
	else if (message == "Could not find the user")
		code = 5;
	else if (message == "User already exists")
		code = 6;
	else if (message == "Wrong password")
		code = 7;
	else if (message == "Wrong value")
		code = 8;
	else
		code = 0;
}
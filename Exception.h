#pragma once
#include <stdexcept>
#include <string>

class Exception : public std::runtime_error
{
public:
	Exception(const std::string& message);

	size_t code;
};

#pragma once

#include <exception>
#include <string>

class UnrecognizedTokenException : public std::exception
{
  public:
    UnrecognizedTokenException()
        : std::exception()
    {
    }

    UnrecognizedTokenException(const std::string& Message)
        : std::exception(Message.c_str())
    {
    }
};

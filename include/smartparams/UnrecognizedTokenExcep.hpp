#pragma once

#include <exception>
#include <string>

class UnrecognizedTokenException : public std::runtime_error
{
  public:
    UnrecognizedTokenException(const std::string& Message)
        : std::runtime_error(Message.c_str())
    {
    }
};

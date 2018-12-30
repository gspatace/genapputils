#pragma once

#include <exception>
#include <string>
class MissingRequiredException : public std::runtime_error
{
  public:
    MissingRequiredException(const std::string& Message)
        : std::runtime_error(Message.c_str())
    {
    }
};

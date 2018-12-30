#pragma once

#include <exception>
#include <string>
class MissingRequiredException : public std::exception
{
  public:
    MissingRequiredException()
        : std::exception()
    {
    }

    MissingRequiredException(const std::string& Message)
        : std::exception(Message.c_str())
    {
    }
};

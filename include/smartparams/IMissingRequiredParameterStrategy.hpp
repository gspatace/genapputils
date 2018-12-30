#pragma once

#include <string>

class IMissingRequiredParameterStrategy
{
  public:
    virtual void Handle(const std::string& Message) = 0;
};

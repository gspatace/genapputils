#pragma once

#include <string>

class IUnrecognizedTokenStrategy
{
  public:
    virtual void Handle(const std::string& Message) = 0;
    virtual ~IUnrecognizedTokenStrategy() = default;
};

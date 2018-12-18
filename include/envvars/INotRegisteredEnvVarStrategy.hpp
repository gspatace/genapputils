#pragma once

#include <string>

class INotRegisteredEnvVarStrategy
{
  public:
    virtual void HandleNotRegisteredEnvVar(const std::string& EnvVarName) = 0;
};

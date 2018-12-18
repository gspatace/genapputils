#pragma once

#include <string>

class IMissingRequiredEnvVarStrategy
{
  public:
    virtual void HandleMissingRequiredEnvVar(const std::string& EnvVarName) = 0;
};
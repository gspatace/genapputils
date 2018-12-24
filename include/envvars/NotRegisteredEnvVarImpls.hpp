#pragma once

#include <iostream>
#include <sstream>

#include "INotRegisteredEnvVarStrategy.hpp"

template<typename ExType>
class ExceptionThrowerNotRegisteredEnvVar : public INotRegisteredEnvVarStrategy
{
  public:
    virtual void HandleNotRegisteredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was not registred, but its usage was attempted.";
        throw ExType(oss.str());
    }
};

class StdErrLoggerNotRegisteredEnvVarHandler : public INotRegisteredEnvVarStrategy
{
  public:
    virtual void HandleNotRegisteredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was not registred, but its usage was attempted.";
        std::cerr << oss.str() << std::endl;
    }
};

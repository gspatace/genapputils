#pragma once

#include <iostream>
#include <sstream>

#include "IMissingRequiredEnvVarStrategy.hpp"

template<typename ExType>
class ExceptionThrowerMissingRequiredHandler : public IMissingRequiredEnvVarStrategy
{
  public:
    virtual void HandleMissingRequiredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was registred as Required, but is missing.";
        throw ExType(oss.str());
    }
};

class StdErrLoggerMissingRequiredEnvVarHandler : public IMissingRequiredEnvVarStrategy
{
  public:
    virtual void HandleMissingRequiredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was registred as Required, but is missing.";
        std::cerr << oss.str() << std::endl;
    }
};
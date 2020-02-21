#pragma once

#include <iostream>
#include <sstream>

#include "IMissingRequiredParameterStrategy.hpp"
#include "MissingRequiredExcep.hpp"

template<typename ExType = MissingRequiredException>
class ExceptionThrowerHandler
{
  public:
    void Handle(const std::string& Parameter) const
    {
        std::ostringstream oss;
        oss << "Parameter <";
        oss << Parameter;
        oss << "> was registered as Required, but was not provided.";
        throw ExType(oss.str());
    }
};

class StdErrLoggerHandler
{
  public:
    void Handle(const std::string& Parameter) const
    {
        std::ostringstream oss;
        oss << "Parameter <";
        oss << Parameter;
        oss << "> was registered as Required, but was not provided.";
        std::cerr << oss.str() << std::endl;
    }
};
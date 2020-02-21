#pragma once

#include <iostream>
#include <sstream>

#include "IUnrecognizedTokenStrategy.hpp"
#include "UnrecognizedTokenExcep.hpp"

template<typename ExType = UnrecognizedTokenException>
class UnrecognizedTokenExceptionThrowerHandler
{
  public:
    void Handle(const std::string& Token) const
    {
        std::ostringstream oss;
        oss << "Unrecognized Token: ";
        oss << Token;
        oss << ".";
        throw ExType(oss.str());
    }
};

class UnrecognizedStdErrLoggerHandler
{
  public:
    void Handle(const std::string& Token) const
    {
        std::ostringstream oss;
        oss << "Unrecognized Token: ";
        oss << Token;
        oss << ".";
        std::cerr << oss.str() << std::endl;
    }
};
#pragma once

#include <iostream>
#include <sstream>

#include "IUnrecognizedTokenStrategy.hpp"
#include "UnrecognizedTokenExcep.hpp"

template<typename ExType = UnrecognizedTokenException>
class UnrecognizedTokenExceptionThrowerHandler : public IUnrecognizedTokenStrategy
{
  public:
    virtual void Handle(const std::string& Token) override
    {
        std::ostringstream oss;
        oss << "Unrecognized Token: ";
        oss << Token;
        oss << ".";
        throw ExType(oss.str());
    }
};

class UnrecognizedStdErrLoggerHandler : public IUnrecognizedTokenStrategy
{
  public:
    virtual void Handle(const std::string& Token) override
    {
        std::ostringstream oss;
        oss << "Unrecognized Token: ";
        oss << Token;
        oss << ".";
        std::cerr << oss.str() << std::endl;
    }
};
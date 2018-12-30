#pragma once

#include <memory>

#include "ParamItem.hpp"
#include "MissingRequiredImpls.hpp"
#include "UnrecognizedTokenImpls.hpp"

class ArgumentParser
{
    using ParamNameComp_t = std::function<bool(const ParamItem& Param, const std::string& Value)>;
    using ParamMap_t = std::map<std::string, ParamItem>;
    using ParamMapConstIter_t = ParamMap_t::const_iterator;
    using ParamVector_t = std::vector<ParamItem>;
    using MissingStrategyHandlerUPtr_t = std::unique_ptr<IMissingRequiredParameterStrategy>;
    using UnrecognizedTokenStrHandlerUPtr_t = std::unique_ptr<IUnrecognizedTokenStrategy>;

  public:
    mDLLIMPORTEXPORT ArgumentParser(const ParamVector_t& Params,
                                    MissingStrategyHandlerUPtr_t MissingHandler = std::make_unique<StdErrLoggerHandler>(),
                                    UnrecognizedTokenStrHandlerUPtr_t BadTokenHandler = std::make_unique<UnrecognizedStdErrLoggerHandler>());

    template<typename RetVal>
    mDLLIMPORTEXPORT RetVal GetValue(const std::string& Name) const;

    mDLLIMPORTEXPORT void Parse(int argc, const char* const argv[]);

  private:
    void CheckRequiredParams() const;
    ParamItem FindByName(const std::string& ParamName, ParamNameComp_t NameComparator);

    ParamNameComp_t ShortNameComparator = [](const ParamItem& Param, const std::string& Value) {
        if (Param.GetShortName() == Value)
            return true;
        return false;
    };

    ParamNameComp_t LongNameComparator = [](const ParamItem& Param, const std::string& Value) {
        if (Param.GetLongName() == Value)
            return true;
        return false;
    };

    ParamVector_t mAllParameters;
    ParamMap_t mProcessedParams;
    MissingStrategyHandlerUPtr_t mMissingParamHandler;
    UnrecognizedTokenStrHandlerUPtr_t mUnTokenHandler;
};

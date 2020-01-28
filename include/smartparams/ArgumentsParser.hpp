#pragma once

#include <memory>

#include "ParamItem.hpp"
#include "MissingRequiredImpls.hpp"
#include "UnrecognizedTokenImpls.hpp"

template 
    <
        class MissingParameterHandlerPolicy = StdErrLoggerHandler,
        class UnrecognizedTokenHandlerPolicy = UnrecognizedStdErrLoggerHandler
    >
class ArgumentParser
{
    using ParamNameComp_t = std::function<bool(const ParamItem& Param, const std::string& Value)>;
    using ParamMap_t = std::map<std::string, ParamItem>;
    using ParamMapConstIter_t = ParamMap_t::const_iterator;
    using ParamVector_t = std::vector<ParamItem>;

  public:
    ArgumentParser(const ParamVector_t& Params)
        : mAllParameters(Params)
    {};

    template<typename RetVal>
    RetVal GetValue(const std::string& Name) const
    {
        ParamMapConstIter_t elem = mProcessedParams.find(Name);
        if (elem != mProcessedParams.end())
            return std::get<RetVal>(elem->second.mValue);

        return {};
    }

    void Parse(int argc, const char* const argv[])
    {
        for (int i = 1; i < argc; ++i)
        {
            std::string str = argv[i];
            if (str.at(0) == '-') //this is a parameter name
            {
                bool isShortName = false;
                if (str.at(1) != '-') //short name
                {
                    isShortName = true;
                }

                //trim leading hyphen(s)
                str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](char c) {
                              if (c == '-')
                                  return false;
                              return true;
                          }));

                ParamItem theParam = isShortName ? FindByName(str, ShortNameComparator) : FindByName(str, LongNameComparator);

                //now the setting
                theParam.IsBoolean() ? theParam.mValue = true : theParam.mValue = std::string(argv[++i]);
                theParam.mIsSet = true;

                //finally push it in the processed map
                mProcessedParams.try_emplace(theParam.GetShortName(), theParam);
            }
            else
            {
                mUnTokenHandler.Handle(str);
            }
        }

        CheckRequiredParams();
    }

  private:
    void CheckRequiredParams() const
    {
        for (const ParamItem& item : mAllParameters)
        {
            if (item.IsRequired())
            {
                const auto& processed = mProcessedParams.find(item.GetShortName());
                if (processed == mProcessedParams.end() ||
                    !processed->second.IsSet())
                {
                    mMissingParamHandler.Handle(item.GetShortName());
                }
            }
        }
    }
    ParamItem FindByName(const std::string& ParamName, ParamNameComp_t NameComparator)
    {
        for (const ParamItem& iter : mAllParameters)
            if (NameComparator(iter, ParamName))
                return iter;

        return {};
    }

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
    MissingParameterHandlerPolicy mMissingParamHandler = {};
    UnrecognizedTokenHandlerPolicy mUnTokenHandler = {};
};

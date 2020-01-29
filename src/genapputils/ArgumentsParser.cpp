#include "ArgumentsParser.hpp"

template mDLLIMPORTEXPORT std::string ArgumentParser::GetValue<std::string>(const std::string& Name) const;
template mDLLIMPORTEXPORT bool ArgumentParser::GetValue<bool>(const std::string& Name) const;

template<typename RetVal>
RetVal ArgumentParser::GetValue(const std::string& Name) const
{
    ParamMapConstIter_t elem = mProcessedParams.find(Name);
    if (elem != mProcessedParams.end())
        return std::get<RetVal>(elem->second.mValue);

    return {};
}

ArgumentParser::ArgumentParser(const ParamVector_t& Params,
                               MissingStrategyHandlerUPtr_t MissingHandler,
                               UnrecognizedTokenStrHandlerUPtr_t BadTokenHandler)
    : mAllParameters(Params)
{
    mMissingParamHandler = std::move(MissingHandler);
    mUnTokenHandler = std::move(BadTokenHandler);
}

void ArgumentParser::Parse(int argc, const char* const argv[])
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

            ParamItem theParam = isShortName ? FindByName(str, ShortNameComparator) : FindByName(str,LongNameComparator);

            //now the setting
            theParam.IsBoolean() ? theParam.mValue = true : theParam.mValue = std::string(argv[++i]);
            theParam.mIsSet = true;

            //finally push it in the processed map
            mProcessedParams.try_emplace(theParam.GetShortName(), theParam);
        }
        else
        {
            mUnTokenHandler->Handle(str);
        }
    }

    CheckRequiredParams();
}

void ArgumentParser::CheckRequiredParams() const
{
    for (const ParamItem& item : mAllParameters)
    {
        if (item.IsRequired())
        {
            const auto& processed = mProcessedParams.find(item.GetShortName());
            if (processed == mProcessedParams.end() ||
                !processed->second.IsSet())
            {
                mMissingParamHandler->Handle(item.GetShortName());
            }
        }
    }
}

ParamItem ArgumentParser::FindByName(const std::string& ParamName, ParamNameComp_t NameComparator)
{
    for (const ParamItem& iter : mAllParameters)
        if (NameComparator(iter, ParamName))
            return iter;

    return {};
}

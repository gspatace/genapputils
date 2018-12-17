#pragma once
#include <algorithm>

#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>
#include <iostream>
#include <variant>

#include "ie_genapputils.hpp"

//EXPIMP template class mDLLIMPORTEXPORT std::allocator<char>;
//EXPIMP template struct mDLLIMPORTEXPORT std::char_traits<char>;
//EXPIMP template class mDLLIMPORTEXPORT  std::basic_string<char>;
////EXPIMP template class mDLLIMPORTEXPORT std::string;
//EXPIMP template class mDLLIMPORTEXPORT std::variant<std::string, bool>;
//EXPIMP template class mDLLIMPORTEXPORT std::_Variant_storage_<true, bool>;


class  ParamItem
{
  public:
    mDLLIMPORTEXPORT ParamItem() = default;
    mDLLIMPORTEXPORT ParamItem(
        const std::string& ParamShortName,
        bool IsRequiredParam = false,
        bool IsBooleanParam = false,
        const std::string& LongParamName = "")
        : mParamShortName(ParamShortName)
        , mIsRequired(IsRequiredParam)
        , mIsBoolean(IsBooleanParam)
        , mParamLongName(LongParamName){};

      mDLLIMPORTEXPORT  ~ParamItem() = default;

    std::string GetShortName() const
    {
        return mParamShortName;
    }
    std::string GetLongName() const
    {
        return mParamLongName;
    }
    bool IsBoolean() const
    {
        return mIsBoolean;
    }
    bool IsRequired() const
    {
        return mIsRequired;
    }
    bool IsSet() const
    {
        return mIsSet;
    }

  private:
    std::string mParamShortName{""};
    std::string mParamLongName{""};
    bool mIsBoolean{false};
    bool mIsSet{false};
    bool mIsRequired{false};
    std::variant<std::string, bool> mValue{false};

    friend class ArgumentParser;
};

class ArgumentParser
{
    using ParamNameComp_t = std::function<bool(const ParamItem& Param, const std::string& Value)>;
    using ParamMap_t = std::map<std::string, ParamItem>;
    using ParamMapConstIter_t = ParamMap_t::const_iterator;
    using ParamVector_t = std::vector<ParamItem>;
    //using ParamVectorConstIter_t = ParamVector_t::const_iterator;
  public:
    mDLLIMPORTEXPORT ArgumentParser(const ParamVector_t& Params)
        : mAllParameters(Params){};

    template<typename RetVal>
     RetVal GetValue(const std::string& Name) const
    {
        ParamMapConstIter_t elem = mProcessedParams.find(Name);
        if (elem != mProcessedParams.end())
            return std::get<RetVal>(elem->second.mValue);

        return {};
    }

    mDLLIMPORTEXPORT void Parse(int argc, const char* const argv[])
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

                ParamItem theParam;
                if (isShortName)
                    theParam = FindByName(str, ShortNameComparator);
                else
                    theParam = FindByName(str, LongNameComparator);

                //now the setting
                if (theParam.IsBoolean())
                    theParam.mValue = true;
                else
                    theParam.mValue = std::string(argv[++i]);

                theParam.mIsSet = true;

                //finally push it in the processed map
                mProcessedParams[theParam.GetShortName()] = theParam;
            }
            else
            {
                std::cerr << "Unrecognized token: " << str << std::endl;
            }
        }

        CheckRequiredParams();
    }

  private:
    void CheckRequiredParams() const
    {
        for (const ParamItem& item : mAllParameters)
        {
            if (item.IsRequired() && !item.IsSet())
            {
                std::ostringstream oss;
                oss << "Parameter ";
                oss << item.GetShortName();
                oss << " is required, but was not given.";
                throw std::logic_error(oss.str());
            }
        }

        //for (ParamVectorConstIter_t iter = mAllParameters.cbegin(); iter != mAllParameters.cend(); ++iter)
        //{
        //	if (iter->IsRequired() && !iter->IsSet())
        //	{
        //		std::ostringstream oss;
        //		oss << "Parameter ";
        //		oss << iter->GetShortName();
        //		oss << " is required, but was not given.";
        //		throw std::logic_error(oss.str());
        //	}
        //}
    }

    ParamItem FindByName(const std::string& ParamName, ParamNameComp_t NameComparator)
    {
        for (const ParamItem& iter : mAllParameters)
            if (NameComparator(iter, ParamName))
                return iter;

        //for (ParamVectorConstIter_t iter = mAllParameters.cbegin(); iter != mAllParameters.cend(); ++iter)
        //{
        //	if (NameComparator(*iter, ParamName))
        //		return *iter;
        //}

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
};


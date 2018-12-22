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

class ParamItem
{
  public:
    ParamItem() = default;
    mDLLIMPORTEXPORT ParamItem(
        const std::string& ParamShortName,
        bool IsRequiredParam = false,
        bool IsBooleanParam = false,
        const std::string& LongParamName = "");

    ParamItem(const ParamItem& Other) = default;
    ~ParamItem() = default;

  private:
    mDLLIMPORTEXPORT std::string GetShortName() const;
    mDLLIMPORTEXPORT std::string GetLongName() const;
    mDLLIMPORTEXPORT bool IsBoolean() const;
    mDLLIMPORTEXPORT bool IsRequired() const;
    mDLLIMPORTEXPORT bool IsSet() const;

  private:
    std::string mParamShortName{""};
    std::string mParamLongName{""};
    bool mIsBoolean{false};
    bool mIsSet{false};
    bool mIsRequired{false};
    std::variant<std::string, bool> mValue{false};

    friend class ArgumentParser;
};

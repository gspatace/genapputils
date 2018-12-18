#pragma once

#include <string>

class EnvVarItem
{
  public:
    EnvVarItem() = default;
    EnvVarItem(const char* const Name, bool IsRequired, const char* const Value)
        : mName(Name)
        , mIsRequired(IsRequired)
        , mValue(Value){};

    ~EnvVarItem() = default;

    std::string GetValue() const
    {
        return mValue;
    }

    bool IsSet() const
    {
        return mIsSet;
    }

    bool IsRequired() const
    {
        return mIsRequired;
    }

  public:
    std::string mName{""};
    std::string mValue;
    bool mIsRequired{false};
    bool mIsSet{false};

    friend class EnvVarCache;
};
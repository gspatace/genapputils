#pragma once

#include <string>

#include "ie_genapputils.hpp"

class EnvVarItem
{
  public:
    EnvVarItem() = default;
    mDLLIMPORTEXPORT EnvVarItem(const char* const Name, bool IsRequired, const char* const Value);
    ~EnvVarItem() = default;

  private:
    std::string GetValue() const;
    bool IsSet() const;
    bool IsRequired() const;
    
    std::string mName{""};
    std::string mValue;
    bool mIsRequired{false};
    bool mIsSet{false};

    friend class EnvVarCache;
};
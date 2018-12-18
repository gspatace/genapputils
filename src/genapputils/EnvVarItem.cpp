#include "EnvVarItem.hpp"

EnvVarItem::EnvVarItem(const char* const Name, bool IsRequired, const char* const Value)
    : mName(Name)
    , mIsRequired(IsRequired)
    , mValue(Value)
{};

std::string EnvVarItem::GetValue() const
{
    return mValue;
}

bool EnvVarItem::IsSet() const
{
    return mIsSet;
}

bool EnvVarItem::IsRequired() const
{
    return mIsRequired;
}
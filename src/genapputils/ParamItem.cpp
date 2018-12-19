#include "ParamItem.hpp"

ParamItem::ParamItem(
    const std::string& ParamShortName,
    bool IsRequiredParam,
    bool IsBooleanParam,
    const std::string& LongParamName)
    : mParamShortName(ParamShortName)
    , mIsRequired(IsRequiredParam)
    , mIsBoolean(IsBooleanParam)
    , mParamLongName(LongParamName)
{};

std::string ParamItem::GetShortName() const
{
    return mParamShortName;
}

std::string ParamItem::GetLongName() const
{
    return mParamLongName;
}

bool ParamItem::IsBoolean() const
{
    return mIsBoolean;
}

bool ParamItem::IsRequired() const
{
    return mIsRequired;
}

bool ParamItem::IsSet() const
{
    return mIsSet;
}
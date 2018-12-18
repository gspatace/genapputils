#include "EnvVarCache.hpp"

EnvVarCache::EnvVarCache()
    : mMissingRequiredHandler(new StdErrLoggerMissingRequiredEnvVarHandler())
    , mNotRegisteredEnvVarHandler(new ExceptionThrowerNotRegisteredEnvVar<std::runtime_error>())
{
}

EnvVarCache& EnvVarCache::Instance()
{
    static EnvVarCache mInstance;
    return mInstance;
}

std::string EnvVarCache::GetValueByKey(const std::string& Key)
{
    std::map<std::string, EnvVarItem>::iterator iter = mEnvVarCache.find(Key);
    if (iter == mEnvVarCache.end())
    {
        mNotRegisteredEnvVarHandler->HandleNotRegisteredEnvVar(Key);
        return "";
    }

    return iter->second.GetValue();
}

void EnvVarCache::Register(const std::vector<EnvVarItem>& EnvVars)
{
    for (std::vector<EnvVarItem>::const_iterator cIter = std::begin(EnvVars); cIter != std::end(EnvVars); ++cIter)
    {
        mEnvVarCache[cIter->mName] = *cIter;
    }
}

void EnvVarCache::GetAll()
{
    for (std::map<std::string, EnvVarItem>::iterator iter = mEnvVarCache.begin(); iter != mEnvVarCache.end(); ++iter)
    {
        if (!iter->second.IsSet())
        { //avoid re-reading an env var if it was already processed
            char* value = nullptr;
            size_t readLength = 0;
            _dupenv_s(&value, &readLength, iter->second.mName.c_str());
            if (readLength > 0)
            {
                iter->second.mValue = value;
                iter->second.mIsSet = true;
            }
        }

        if (iter->second.IsRequired() && !iter->second.IsSet())
        {
            mMissingRequiredHandler->HandleMissingRequiredEnvVar(iter->second.mName);
        }
    }
}

void EnvVarCache::RegisterMissingRequiredHandler(std::unique_ptr<IMissingRequiredEnvVarStrategy> MissingReqHandler)
{
    mMissingRequiredHandler = std::move(MissingReqHandler);
}

void EnvVarCache::RegisterNotRegisteredEnvVarHandler(std::unique_ptr<INotRegisteredEnvVarStrategy> NotRegisteredHandler)
{
    mNotRegisteredEnvVarHandler = std::move(NotRegisteredHandler);
}
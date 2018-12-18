#pragma once
#include <map>
#include <vector>
#include <string>
#include <sstream>

#include "EnvVarItem.hpp"
#include "MissingRequiredEnvVarImpls.hpp"
#include "NotRegisteredEnvVarImpls.hpp"

class EnvVarCache
{
  public:
    static EnvVarCache& Instance()
    {
        static EnvVarCache mInstance;
        return mInstance;
    }
    std::string GetValueByKey(const std::string& Key)
    {
        std::map<std::string, EnvVarItem>::iterator iter = mEnvVarCache.find(Key);
        if (iter == mEnvVarCache.end())
        {
            mNotRegisteredEnvVarHandler->HandleNotRegisteredEnvVar(Key);
            return "";
        }

        return iter->second.GetValue();
    }

  private:
    EnvVarCache()
        : mMissingRequiredHandler(new StdErrLoggerMissingRequiredEnvVarHandler())
        , mNotRegisteredEnvVarHandler(new ExceptionThrowerNotRegisteredEnvVar<std::runtime_error>())
    {
    }

    void Register(const std::vector<EnvVarItem>& EnvVars)
    {
        for (std::vector<EnvVarItem>::const_iterator cIter = std::begin(EnvVars); cIter != std::end(EnvVars); ++cIter)
        {
            mEnvVarCache[cIter->mName] = *cIter;
        }
    }

    void GetAll()
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

    void RegisterMissingRequiredHandler(std::unique_ptr<IMissingRequiredEnvVarStrategy> MissingReqHandler)
    {
        mMissingRequiredHandler = std::move(MissingReqHandler);
    }

    void RegisterNotRegisteredEnvVarHandler(std::unique_ptr<INotRegisteredEnvVarStrategy> NotRegisteredHandler)
    {
        mNotRegisteredEnvVarHandler = std::move(NotRegisteredHandler);
    }

    std::map<std::string, EnvVarItem> mEnvVarCache;
    std::unique_ptr<IMissingRequiredEnvVarStrategy> mMissingRequiredHandler;
    std::unique_ptr<INotRegisteredEnvVarStrategy> mNotRegisteredEnvVarHandler;

    friend class EnvVarRegistrar;
};

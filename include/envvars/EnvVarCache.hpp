#pragma once
#include <map>
#include <vector>
#include <string>
#include <sstream>

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

class IMissingRequiredEnvVarStrategy
{
  public:
    virtual void HandleMissingRequiredEnvVar(const std::string& EnvVarName) = 0;
};

class INotRegisteredEnvVarStrategy
{
  public:
    virtual void HandleNotRegisteredEnvVar(const std::string& EnvVarName) = 0;
};

template<typename ExType>
class ExceptionThrowerMissingRequiredHandler : public IMissingRequiredEnvVarStrategy
{
  public:
    virtual void HandleMissingRequiredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was registred as Required, but is missing.";
        throw ExType(oss.str());
    }
};

template<typename ExType>
class ExceptionThrowerNotRegisteredEnvVar : public INotRegisteredEnvVarStrategy
{
  public:
    virtual void HandleNotRegisteredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was not registred, but its usage was attempted.";
        throw ExType(oss.str());
    }
};

class StdErrLoggerMissingRequiredEnvVarHandler : public IMissingRequiredEnvVarStrategy
{
  public:
    virtual void HandleMissingRequiredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was registred as Required, but is missing.";
        std::cerr << oss.str() << std::endl;
    }
};

class StdErrLoggerNotRegisteredEnvVarHandler : public INotRegisteredEnvVarStrategy
{
  public:
    virtual void HandleNotRegisteredEnvVar(const std::string& EnvVarName) override
    {
        std::ostringstream oss;
        oss << "Environment Variable <";
        oss << EnvVarName;
        oss << "> was not registred, but its usage was attempted.";
        std::cerr << oss.str() << std::endl;
    }
};

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

class EnvVarRegistrar final
{
  public:
    static void RegisterAppEnvVars(const std::vector<EnvVarItem>& EnvVars,
                                   std::unique_ptr<IMissingRequiredEnvVarStrategy> ErrHandler = nullptr,
                                   std::unique_ptr<INotRegisteredEnvVarStrategy> NotRegisteredErrHandler = nullptr)
    {
        EnvVarCache::Instance().Register(EnvVars);
        if (nullptr != ErrHandler.get())
            EnvVarCache::Instance().RegisterMissingRequiredHandler(std::move(ErrHandler));
        if (nullptr != NotRegisteredErrHandler.get())
            EnvVarCache::Instance().RegisterNotRegisteredEnvVarHandler(std::move(NotRegisteredErrHandler));
        EnvVarCache::Instance().GetAll();
    }
};

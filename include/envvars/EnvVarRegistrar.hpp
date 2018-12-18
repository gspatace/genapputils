#pragma once

#include <vector>
#include <memory>
#include "EnvVarCache.hpp"

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
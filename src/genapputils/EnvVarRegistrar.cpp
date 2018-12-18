#include "EnvVarRegistrar.hpp"

void EnvVarRegistrar::RegisterAppEnvVars(const std::vector<EnvVarItem>& EnvVars,
                                         std::unique_ptr<IMissingRequiredEnvVarStrategy> ErrHandler,
                                         std::unique_ptr<INotRegisteredEnvVarStrategy> NotRegisteredErrHandler)
{
    EnvVarCache::Instance().Register(EnvVars);
    if (nullptr != ErrHandler.get())
        EnvVarCache::Instance().RegisterMissingRequiredHandler(std::move(ErrHandler));
    if (nullptr != NotRegisteredErrHandler.get())
        EnvVarCache::Instance().RegisterNotRegisteredEnvVarHandler(std::move(NotRegisteredErrHandler));
    EnvVarCache::Instance().GetAll();
}
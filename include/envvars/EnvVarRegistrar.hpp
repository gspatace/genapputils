#pragma once

#include <vector>
#include <memory>
#include "EnvVarCache.hpp"

#include "ie_genapputils.hpp"

class mDLLIMPORTEXPORT EnvVarRegistrar
{
  public:
    static void RegisterAppEnvVars(const std::vector<EnvVarItem>& EnvVars,
                                   std::unique_ptr<IMissingRequiredEnvVarStrategy> ErrHandler = nullptr,
                                   std::unique_ptr<INotRegisteredEnvVarStrategy> NotRegisteredErrHandler = nullptr);
};
#pragma once
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "EnvVarItem.hpp"
#include "MissingRequiredEnvVarImpls.hpp"
#include "NotRegisteredEnvVarImpls.hpp"

#include "ie_genapputils.hpp"

class EnvVarCache
{
  public:
    static mDLLIMPORTEXPORT EnvVarCache& Instance();
    std::string mDLLIMPORTEXPORT GetValueByKey(const std::string& Key);

  private:
    EnvVarCache();
    void Register(const std::vector<EnvVarItem>& EnvVars);
    void GetAll();
    void RegisterMissingRequiredHandler(std::unique_ptr<IMissingRequiredEnvVarStrategy> MissingReqHandler);
    void RegisterNotRegisteredEnvVarHandler(std::unique_ptr<INotRegisteredEnvVarStrategy> NotRegisteredHandler);

    std::map<std::string, EnvVarItem> mEnvVarCache;
    std::unique_ptr<IMissingRequiredEnvVarStrategy> mMissingRequiredHandler;
    std::unique_ptr<INotRegisteredEnvVarStrategy> mNotRegisteredEnvVarHandler;

    friend class EnvVarRegistrar;
};

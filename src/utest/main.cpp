
#include <iostream>

#include "EnvVarCache.hpp"
#include "ParamItem.hpp"
#include "ie_genapputils.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello World!\n";

    const std::vector<EnvVarItem> allEnvVars = {
        {"ONE_ENV_VAR", true, ""},
        {"ANOTHER_ENV_VAR", false, ""}};

    try
    {
        EnvVarRegistrar::RegisterAppEnvVars({{"ONE_ENV_VAR", true, ""},
                                             {"REALLY_IMPORTANT_ENV_VAR", true, ""},
                                             {"ANOTHER_ENV_VAR", false, ""}},
                                            std::make_unique<StdErrLoggerMissingRequiredEnvVarHandler>(),
                                            std::make_unique<StdErrLoggerNotRegisteredEnvVarHandler>());
    }
    catch (std::exception& Ex)
    {
        std::cerr << Ex.what() << std::endl;
    }
    ////EnvVarCache envVarCache(allEnvVars);
    EnvVarCache::Instance().GetValueByKey("BLIZZ_API_KEY");
    EnvVarCache::Instance().GetValueByKey("REQUEST_DELAY");
    EnvVarCache::Instance().GetValueByKey("BLIZZ_API_KEY");
    EnvVarCache::Instance().GetValueByKey("BLIZZ_API_KEY");
    EnvVarCache::Instance().GetValueByKey("ONE_ENV_VAR");

    const std::vector<ParamItem> appParams = {
        {"P1VALUE", false},
        {"P2BOOL", true},
        {"P3VALUE", false},
        {"P4VALUE", false, "PARAM4_VALUE"},
        {"P5BOOL", true}};

    ArgumentParser argParser(appParams);
    argParser.Parse(argc, argv);

    std::string p1Value = argParser.GetValue<std::string>("P3VALUE");

    //EnvVarCache<EnvVarRegistrar>::Instance().GetValueByKey("something");
}

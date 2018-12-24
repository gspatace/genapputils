
#include <iostream>

#include "EnvVarCache.hpp"
#include "ArgumentsParser.hpp"
#include "EnvVarRegistrar.hpp"
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
                                             {"BLIZZ_API_KEY", true, ""},
                                             {"ANOTHER_ENV_VAR", false, ""}},
                                            std::make_unique<StdErrLoggerMissingRequiredEnvVarHandler>(),
                                            std::make_unique<StdErrLoggerNotRegisteredEnvVarHandler>());
    }
    catch (std::exception& Ex)
    {
        std::cerr << Ex.what() << std::endl;
    }
    ////EnvVarCache envVarCache(allEnvVars);
    EnvVarCache::Instance().GetValueByKey("REALLY_IMPORTANT_ENV_VAR");
    EnvVarCache::Instance().GetValueByKey("ANOTHER_ENV_VAR");
    EnvVarCache::Instance().GetValueByKey("BLIZZ_API_KEY");
    EnvVarCache::Instance().GetValueByKey("BLIZZ_API_KEY");
    EnvVarCache::Instance().GetValueByKey("ONE_ENV_VAR");

    const std::vector<ParamItem> appParams = {
        {"P1VALUE", true},
        {"P2BOOL", false, true},
        {"P3VALUE", true},
        {"P4VALUE", false, false, "PARAM4_VALUE"},
        {"P5BOOL", true, true},
        {"P6VALUE", true}};

    ArgumentParser argParser(appParams);

    try
    {
        argParser.Parse(argc, argv);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    std::string p1Value = argParser.GetValue<std::string>("P3VALUE");
    std::cout << p1Value << std::endl;
    bool p2Value = argParser.GetValue<bool>("P2BOOL");
    std::cout << p2Value << std::endl;
}

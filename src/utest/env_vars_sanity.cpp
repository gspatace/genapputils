#include "EnvVarCache.hpp"
#include "EnvVarRegistrar.hpp"

#include "gtest/gtest.h"

namespace testing
{
TEST(EnvVarSanity, All_EnvVars_Requested_Present)
{
    EnvVarRegistrar::RegisterAppEnvVars({{"ONE_ENV_VAR", true, ""},
                                         {"REALLY_IMPORTANT_ENV_VAR", true, ""},
                                         {"SOME_API_KEY", true, ""}},
                                        std::make_unique<StdErrLoggerMissingRequiredEnvVarHandler>(),
                                        std::make_unique<StdErrLoggerNotRegisteredEnvVarHandler>());

    std::string oneEnvVar = EnvVarCache::Instance().GetValueByKey("ONE_ENV_VAR");
    EXPECT_EQ(oneEnvVar, "one env var value");

    std::string reallyImportant = EnvVarCache::Instance().GetValueByKey("REALLY_IMPORTANT_ENV_VAR");
    EXPECT_EQ(reallyImportant, "really important env var");

    std::string someApiKey = EnvVarCache::Instance().GetValueByKey("SOME_API_KEY");
    EXPECT_EQ(someApiKey, "abcdefghijkl");
}
} // namespace testing
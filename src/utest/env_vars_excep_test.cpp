#include "EnvVarCache.hpp"
#include "EnvVarRegistrar.hpp"

#include "gtest/gtest.h"

namespace testing
{
TEST(EnvVarSanity, EnvVarsReqMissingExcep)
{
    EXPECT_THROW({
        try
        {
            EnvVarRegistrar::RegisterAppEnvVars({{"ONE_ENV_VAR", true, ""},
                                                 {"REQUIRED_BUT_NOT_PRESENT", true, ""}},
                                                std::make_unique<ExceptionThrowerMissingRequiredHandler<std::logic_error>>(),
                                                std::make_unique<StdErrLoggerNotRegisteredEnvVarHandler>());
        }
        catch (const std::logic_error& ex)
        {
            std::string exText = "Environment Variable <REQUIRED_BUT_NOT_PRESENT> was registred as Required, but is missing.";
            EXPECT_STREQ(exText.c_str(), ex.what());
            throw;
        }
    }, std::logic_error);
}
} // namespace testing
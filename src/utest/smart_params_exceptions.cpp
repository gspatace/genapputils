#include "ArgumentsParser.hpp"
#include "MissingRequiredExcep.hpp"
#include "gtest/gtest.h"

int passthru_argc;
char** passthru_argv;

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    passthru_argc = argc;
    passthru_argv = argv;
    return RUN_ALL_TESTS();
}

namespace testing
{
TEST(SmartParams, SmartParamsMyException)
{
    const std::vector<ParamItem> allParams = {
        {"PARAM", true}};

    ArgumentParser argParser(allParams, std::make_unique<ExceptionThrowerHandler<>>());
    EXPECT_THROW({
        try
        {
            argParser.Parse(passthru_argc, passthru_argv);
        }
        catch (const MissingRequiredException& Excep)
        {
            std::string expectedText = "Parameter <PARAM> was registered as Required, but was not provided.";
            ASSERT_STREQ(expectedText.c_str(), Excep.what());
            throw;
        }
    },
                 MissingRequiredException);
}

TEST(SmartParams, SmartParamsStdLogicError)
{
    const std::vector<ParamItem> allParams = {
        {"PARAM", true}};

    ArgumentParser argParser(allParams, std::make_unique<ExceptionThrowerHandler<std::logic_error>>());
    EXPECT_THROW({
        try
        {
            argParser.Parse(passthru_argc, passthru_argv);
        }
        catch (const std::logic_error& Excep)
        {
            std::string expectedText = "Parameter <PARAM> was registered as Required, but was not provided.";
            ASSERT_STREQ(expectedText.c_str(), Excep.what());
            throw;
        }
    },
                 std::logic_error);
}
} // namespace testing
#include "ArgumentsParser.hpp"
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
TEST(SmartParams, SmartParamsUnrecognizedTokenStderrMessage)
{
    const std::vector<ParamItem> allParams = {
        {"P1VALUE", true}};

    ArgumentParser argParser(allParams);
    ::testing::internal::CaptureStderr();
    argParser.Parse(passthru_argc, passthru_argv);
    std::string capturedStderrStream = ::testing::internal::GetCapturedStderr();
    std::string expectedStderrStream = "Unrecognized Token: BadToken.\n";
    ASSERT_STREQ(capturedStderrStream.c_str(), expectedStderrStream.c_str());
}
} // namespace testing
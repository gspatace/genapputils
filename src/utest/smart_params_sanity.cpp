#include "ArgumentsParser.hpp"

#include "gtest/gtest.h"

// Keep argc, argv
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
TEST(SmartParams, SmartParamsSanity)
{
    const std::vector<ParamItem> appParams = {
        {"P1VALUE", true},
        {"P2BOOL", false, true},
        {"P3VALUE", true},
        {"P4VALUE", false, false, "PARAM4_VALUE"},
        {"P5BOOL", true, true},
        {"P6VALUE", false}};

    ArgumentParser argParser(appParams);

    try
    {
        argParser.Parse(passthru_argc, passthru_argv);
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    std::string p1Val = argParser.GetValue<std::string>("P1VALUE");
    ASSERT_EQ(p1Val, "v1");

    bool p2Passed = argParser.GetValue<bool>("P2BOOL");
    ASSERT_EQ(p2Passed, true);

    std::string p3Val = argParser.GetValue<std::string>("P3VALUE");
    ASSERT_EQ(p3Val, "v3");

    std::string p4Val = argParser.GetValue<std::string>("P4VALUE");
    ASSERT_EQ(p4Val, "p4value");

    bool p5Passed = argParser.GetValue<bool>("P5BOOL");
    ASSERT_EQ(p5Passed, true);
}
} // namespace testing

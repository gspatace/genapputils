cmake_minimum_required (VERSION 3.8)

include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories("${PROJECT_SOURCE_DIR}/include/envvars")
include_directories("${PROJECT_SOURCE_DIR}/include/smartparams")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
SET(EXTRALIBS "")
else()
SET(EXTRALIBS "pthread")
endif()

include_directories("${GOOGLETEST_INC}")
link_directories("${GOOGLETEST_LIB}")

add_executable(app main.cpp)
target_link_libraries(app genapputils)

add_executable(env_vars_sanity env_vars_sanity.cpp)
target_link_libraries( env_vars_sanity genapputils gtest gtest_main "${EXTRALIBS}" )

add_test( NAME EnvVarsSanity WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" COMMAND env_vars_sanity)
set_tests_properties( EnvVarsSanity PROPERTIES 
    ENVIRONMENT 
        "ONE_ENV_VAR=one env var value;SOME_API_KEY=abcdefghijkl;REALLY_IMPORTANT_ENV_VAR=really important env var"
)

add_executable( envVarsExcep env_vars_excep_test.cpp)
target_link_libraries( envVarsExcep genapputils gtest gtest_main "${EXTRALIBS}" )
add_test( NAME EnvVarsReqMissingEx WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" COMMAND envVarsExcep)
set_tests_properties( EnvVarsReqMissingEx PROPERTIES 
    ENVIRONMENT 
        "ONE_ENV_VAR=one env var value"
)

add_executable( smartParamsSanity smart_params_sanity.cpp)
target_link_libraries( smartParamsSanity genapputils gtest "${EXTRALIBS}" )
add_test( NAME SmartParamsSanity WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" COMMAND smartParamsSanity -P1VALUE v1 -P2BOOL -P3VALUE v3 --PARAM4_VALUE p4value -P5BOOL )

add_executable( smartParamsMyException smart_params_exceptions.cpp )
target_link_libraries( smartParamsMyException genapputils gtest "${EXTRALIBS}")
add_test( NAME SmartParamsMyException WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" COMMAND smartParamsMyException )

add_executable( smartParamsMissingStderrLogging smart_params_missing_stderr_logging.cpp )
target_link_libraries( smartParamsMissingStderrLogging genapputils gtest "${EXTRALIBS}" )
add_test( NAME SmartParamsMissingStderrLogging WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" COMMAND smartParamsMissingStderrLogging )

add_executable( smartParamsBadTokenStderrLogging smart_params_badtoken_stderr_logging.cpp )
target_link_libraries( smartParamsBadTokenStderrLogging genapputils gtest "${EXTRALIBS}" )
add_test( NAME SmartParamsBadTokenStderrLogging WORKING_DIRECTORY "${CMAKE_BINARY_DIR}" COMMAND smartParamsBadTokenStderrLogging -P1VALUE v1 BadToken )